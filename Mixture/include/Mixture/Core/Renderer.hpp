#pragma once

#include "Mixture/Core/Base.hpp"

#include "Platform/Vulkan/Context.hpp"
#include "Platform/Vulkan/GraphicsPipeline.hpp"

#include "Platform/Vulkan/Buffer/Index.hpp"
#include "Platform/Vulkan/Buffer/Vertex.hpp"

namespace Mixture
{
	class Renderer
	{
	public:
		static void Init(const std::string& applicationName);
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void DrawFrame();
		static void OnEvent(Event& e);

	private:
		static Vulkan::Context& s_VulkanContext;

		static Scope<Vulkan::GraphicsPipeline> s_GraphicsPipeline;
		static Scope<Vulkan::IndexBuffer> s_IndexBuffer;
		static Scope<Vulkan::VertexBuffer> s_VertexBuffer;
	};
}