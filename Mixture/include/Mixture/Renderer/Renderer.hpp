#pragma once

#include "Mixture/Core/Base.hpp"

#include "Mixture/Renderer/LayerStack.hpp"
#include "Mixture/Renderer/Shapes/ShapeRenderer.hpp"
#include "Mixture/Renderer/ImGui/ImGuiRenderer.hpp"

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

		static void OnFramebufferResize(uint32_t width, uint32_t height);

		static void DrawFrame(FrameInfo& frameInfo, const LayerStack& layerStack);
        
        /**
         * Returns if the Viewport is currently 'targeted' by the mouse
         */
        static bool DrawImGuiViewport();
        
        static ShapeRenderer& Shapes() { return *s_ShapeRenderer; }

	private:
		static Vulkan::Context& s_VulkanContext;
        
        static Scope<ShapeRenderer> s_ShapeRenderer;
        static Scope<ImGuiRenderer> s_ImGuiRenderer;
	};
}
