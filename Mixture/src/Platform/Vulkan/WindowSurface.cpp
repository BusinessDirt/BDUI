#include "mxpch.hpp"
#include "Platform/Vulkan/WindowSurface.hpp"

#include "Mixture/Core/Application.hpp"

namespace Vulkan
{
    WindowSurface::WindowSurface(const VkInstance instance)
        : m_Instance(instance)
    {
        const Mixture::Window& window = Mixture::Application::Get().GetWindow();
        m_Surface = window.CreateVulkanSurface(instance);
    }

    WindowSurface::~WindowSurface()
    {
        if (m_Surface)
        {
            vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
            m_Surface = VK_NULL_HANDLE;
        }
    }
}
