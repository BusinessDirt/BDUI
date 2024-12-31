#include "mxpch.hpp"
#include "Platform/Linux/LinuxWindow.hpp"
#ifdef OPAL_PLATFORM_LINUX
#include <stdexcept>

namespace Mixture 
{

    LinuxWindow::LinuxWindow(const WindowProps& props)
    {
        Init(props);
    }

    LinuxWindow::~LinuxWindow() 
    {
        Shutdown();
    }

    void LinuxWindow::Init(const WindowProps& props) 
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        m_Display = XOpenDisplay(nullptr);
        if (!m_Display)
            throw std::runtime_error("Failed to open X display!");

        int screen = DefaultScreen(m_Display);
        m_WindowHandle = XCreateSimpleWindow(m_Display, RootWindow(m_Display, screen),
                                             10, 10, m_Data.Width, m_Data.Height,
                                             1, BlackPixel(m_Display, screen), WhitePixel(m_Display, screen));

        if (!m_WindowHandle)
            throw std::runtime_error("Failed to create X11 window!");

        XStoreName(m_Display, m_WindowHandle, m_Data.Title.c_str());
        XMapWindow(m_Display, m_WindowHandle);
    }

    void LinuxWindow::Shutdown() 
    {
        if (m_Display && m_WindowHandle)
        {
            XDestroyWindow(m_Display, m_WindowHandle);
            XCloseDisplay(m_Display);
        }
    }

    void LinuxWindow::OnUpdate() 
    {
        XEvent event;
        while (XPending(m_Display)) 
        {
            XNextEvent(m_Display, &event);
        }
    }

    void LinuxWindow::GetFramebufferSize(int* width, int* height) const
    {
        *width = GetWidth();
        *height = GetHeight();
    }

    VkSurfaceKHR LinuxWindow::CreateVulkanSurface(VkInstance instance) const override
    {
        VkXlibSurfaceCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        createInfo.dpy = m_Display;
        createInfo.window = m_Window;

        VkSurfaceKHR surface;
        if (vkCreateXlibSurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) 
        {
            OPAL_CORE_ERROR("Failed to create Vulkan surface for Linux!");
        }

        return surface;
    }
}

#endif
