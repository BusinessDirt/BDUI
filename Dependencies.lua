-- Dependencies
IncludeDir = {}
IncludeDir["spdlog"] = "%{wks.location}/vendor/spdlog/include"
IncludeDir["Vulkan"] = os.getenv("VULKAN_SDK") .. "/Include"
IncludeDir["glm"] = "%{wks.location}/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/vendor/stb_image"

LibraryDir = {}
LibraryDir["Vulkan"] = os.getenv("VULKAN_SDK") .. "/Lib"

Library = {}
Library["Vulkan"] = "vulkan-1"
Library["MoltenVK"] = "MoltenVK"

if os.istarget("windows") then
    Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
    Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
    Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
    Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

    Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
    Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
    Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"
end
