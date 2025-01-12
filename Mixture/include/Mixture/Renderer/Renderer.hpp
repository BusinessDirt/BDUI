#pragma once

#include "Mixture/Core/Base.hpp"
#include "Mixture/Renderer/LayerStack.hpp"

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

		static void PushLayer(Layer* layer) { s_LayerStack->PushLayer(layer); }

	private:
		static Vulkan::Context& s_VulkanContext;
		static Scope<LayerStack> s_LayerStack;
	};
}