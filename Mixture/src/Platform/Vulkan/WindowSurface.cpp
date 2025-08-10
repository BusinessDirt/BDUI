#include "mxpch.hpp"
#include "Platform/Vulkan/WindowSurface.hpp"

#include "Platform/Vulkan/Context.hpp"

#include "Mixture/Core/Application.hpp"

namespace Mixture::Vulkan
{
    WindowSurface::WindowSurface()
    {
        const Mixture::Window& window = Mixture::Application::Get().GetWindow();
        window.CreateVulkanSurface(Context::Get().Instance().GetHandle(), nullptr, &m_Surface);
    }

    WindowSurface::~WindowSurface()
    {
        if (m_Surface)
        {
            vkDestroySurfaceKHR(Context::Get().Instance().GetHandle(), m_Surface, nullptr);
            m_Surface = VK_NULL_HANDLE;
        }
    }
}
