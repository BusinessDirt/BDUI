#include "mxpch.hpp"
#include "Mixture/Assets/ShaderCompiler.hpp"

#include "Platform/Vulkan/Base.hpp"

#include "Mixture/Util/Util.hpp"
#include "Mixture/Util/ToString.hpp"

#include "Mixture/Core/Application.hpp"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>

namespace Mixture
{
    namespace Util
    {
        static shaderc_shader_kind ShaderStageToShaderC(ShaderStage stage)
        {
            if (stage == SHADER_STAGE_VERTEX) return shaderc_vertex_shader;
            if (stage == SHADER_STAGE_FRAGMENT) return shaderc_fragment_shader;

            OPAL_CORE_ERROR("Unsupported shader type");
            return shaderc_vertex_shader; // these issues should be catched at runtime so returning this shouldn't be a problem
        }

        static VkFormat GetVkFormat(const spirv_cross::SPIRType& type)
        {
            if (type.basetype == spirv_cross::SPIRType::Float)
            {
                if (type.vecsize == 1) return VK_FORMAT_R32_SFLOAT;
                if (type.vecsize == 2) return VK_FORMAT_R32G32_SFLOAT;
                if (type.vecsize == 3) return VK_FORMAT_R32G32B32_SFLOAT;
                if (type.vecsize == 4) return VK_FORMAT_R32G32B32A32_SFLOAT;
            }
            if (type.basetype == spirv_cross::SPIRType::Int)
            {
                if (type.vecsize == 1) return VK_FORMAT_R32_SINT;
                if (type.vecsize == 2) return VK_FORMAT_R32G32_SINT;
                if (type.vecsize == 3) return VK_FORMAT_R32G32B32_SINT;
                if (type.vecsize == 4) return VK_FORMAT_R32G32B32A32_SINT;
            }
            if (type.basetype == spirv_cross::SPIRType::UInt)
            {
                if (type.vecsize == 1) return VK_FORMAT_R32_UINT;
                if (type.vecsize == 2) return VK_FORMAT_R32G32_UINT;
                if (type.vecsize == 3) return VK_FORMAT_R32G32B32_UINT;
                if (type.vecsize == 4) return VK_FORMAT_R32G32B32A32_UINT;
            }

            OPAL_CORE_ERROR("Unsupported Format");
            return VK_FORMAT_UNDEFINED;
        }
    }
	namespace ShaderCompiler
	{
		SPVShader Compile(const std::string& shaderName, const Flags& compileFlags)
		{
            const std::filesystem::path shaderPath = Application::Get().GetAssetManager().GetShaderPath();
            SPVShader spvShader{};

            // Get a list of files for the given compiler stage flags
            Vector<std::filesystem::path> shaderFiles = Vector<std::filesystem::path>();
            switch (compileFlags.PipelineType)
            {
            case GRAPHICS_PIPELINE:
            {
                // .vert and .frag
                shaderFiles.push_back(shaderPath / (shaderName + ".vert"));
                shaderFiles.push_back(shaderPath / (shaderName + ".frag"));
            }
            }

            // compile every stage
            for (const auto& shaderFile : shaderFiles)
            {
                ShaderStage stage = Util::FilePathToShaderStage(shaderFile);
                spvShader.Data[stage] = CompileStage(shaderFile, compileFlags, stage);
            }

            Reflect(spvShader);
            if (compileFlags.Debug) DebugPrint(shaderName, compileFlags, spvShader);

            return spvShader;
		}

		void Reflect(SPVShader& shader)
		{
            spirv_cross::Compiler vertCompiler(shader.Data.at(SHADER_STAGE_VERTEX));
            spirv_cross::ShaderResources vertResources = vertCompiler.get_shader_resources();

            // Push
            shader.PushConstant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            for (const spirv_cross::Resource& resource : vertResources.push_constant_buffers)
            {
                const spirv_cross::SPIRType& bufferType = vertCompiler.get_type(resource.base_type_id);
                shader.PushConstant.size = static_cast<uint32_t>(vertCompiler.get_declared_struct_size(bufferType));
                vertCompiler.get_binary_offset_for_decoration(resource.id, spv::DecorationBinding, shader.PushConstant.offset);
            }

            // Sort resources by location
            // this is required to calculate the offsets properly
            std::vector<spirv_cross::Resource> stageInputs(vertResources.stage_inputs.begin(), vertResources.stage_inputs.end());
            std::sort(stageInputs.begin(), stageInputs.end(), [&vertCompiler](const spirv_cross::Resource& a, const spirv_cross::Resource& b)
                {
                    uint32_t locationA = vertCompiler.get_decoration(a.id, spv::DecorationLocation);
                    uint32_t locationB = vertCompiler.get_decoration(b.id, spv::DecorationLocation);
                    return locationA < locationB;
                });

            std::unordered_map<uint32_t, uint32_t> bindingStrides;

            for (const spirv_cross::Resource& resource : stageInputs)
            {
                uint32_t location = vertCompiler.get_decoration(resource.id, spv::DecorationLocation);
                uint32_t binding = vertCompiler.get_decoration(resource.id, spv::DecorationBinding);
                auto type = vertCompiler.get_type(resource.base_type_id);

                VkFormat format = Util::GetVkFormat(type);
                uint32_t size = type.vecsize * 4; // Assuming 4 bytes per component (float, int, uint)

                VkVertexInputAttributeDescription attributeDescription{};
                attributeDescription.location = location;
                attributeDescription.binding = binding;
                attributeDescription.format = format;
                attributeDescription.offset = bindingStrides[binding];
                shader.VertexInputAttributes.emplace_back(attributeDescription);

                bindingStrides[binding] += size;
            }

            for (const auto& [binding, stride] : bindingStrides)
            {
                VkVertexInputBindingDescription bindingDescription{};
                bindingDescription.binding = binding;
                bindingDescription.stride = stride;
                bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
                shader.VertexInputBindings.emplace_back(bindingDescription);
            }
		}

        void DebugPrint(const std::string& shaderName, const Flags& compileFlags, SPVShader& shader)
        {
            VULKAN_INFO_BEGIN("Compiled Shader");
            // General Information
            VULKAN_INFO_LIST("Name: {}", 0, shaderName);
            VULKAN_INFO_LIST("Pipeline Type: {}", 0, (compileFlags.PipelineType == GRAPHICS_PIPELINE ? "Graphics" : "Unknown"));

            // Push Constant
            if (shader.PushConstant.size > 0)
            {
                VULKAN_INFO_LIST_HEADER("Push Constant", 0);
                VULKAN_INFO_LIST("Size: {}", 1, shader.PushConstant.size);
                VULKAN_INFO_LIST("Offset: {}", 1, shader.PushConstant.offset);
            }

            // Vertex Input Bindings
            if (shader.VertexInputBindings.size() > 0)
            {
                VULKAN_INFO_LIST_HEADER("Vertex Input Bindings", 0);
                
                for (const auto& binding : shader.VertexInputBindings)
                {
                    VULKAN_INFO_LIST("Binding {}:", 1, binding.binding);
                    VULKAN_INFO_LIST("Stride: {}", 2, binding.stride);
                }
            }

            // Vertex Input Attributes
            if (shader.VertexInputAttributes.size() > 0)
            {
                VULKAN_INFO_LIST_HEADER("Vertex Attributes", 0);

                for (const auto& attribute : shader.VertexInputAttributes)
                {
                    VULKAN_INFO_LIST("Location {}:", 1, attribute.location);
                    VULKAN_INFO_LIST("Binding: {}", 2, attribute.binding);
                    VULKAN_INFO_LIST("Format: {}", 2, Vulkan::ToString::Format(attribute.format));
                    VULKAN_INFO_LIST("Offset: {}", 2, attribute.offset);
                }
            }

            VULKAN_INFO_END();
        }

		std::vector<uint32_t> CompileStage(const std::filesystem::path& path, const Flags& compileFlags, ShaderStage stage)
		{
            shaderc::Compiler compiler;
            shaderc::CompileOptions options;
            shaderc_shader_kind shaderKind = Util::ShaderStageToShaderC(stage);

            options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);

            std::string source = ReadFile(path);

            auto preProcessed = compiler.PreprocessGlsl(source, shaderKind, path.filename().string().c_str(), options);
            auto compiled = compiler.CompileGlslToSpv(source, shaderKind, path.filename().string().c_str(), options);


            if (compiled.GetCompilationStatus() != shaderc_compilation_status_success)
            {
                OPAL_CORE_ERROR(compiled.GetErrorMessage());
                OPAL_CORE_ASSERT(false);
            }

            return std::vector<uint32_t>(compiled.cbegin(), compiled.cend());
		}
	}
}