#pragma once

#include "Mixture/Core/Base.hpp"

#include "Platform/Vulkan/Context.hpp"

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
	};
}