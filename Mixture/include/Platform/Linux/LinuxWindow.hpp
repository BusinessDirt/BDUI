#pragma once
#include <Opal/Base.hpp>
#ifdef OPAL_PLATFORM_LINUX

#include "Mixture/Core/Window.hpp"
#include <X11/Xlib.h>

namespace Mixture {

    class LinuxWindow : public Window
    {
    public:
        LinuxWindow(const WindowProps& props);
        ~LinuxWindow();

        void OnUpdate() override;
        int GetWidth() const override { return m_Data.Width; }
        int GetHeight() const override { return m_Data.Height; }
        
        void* GetNativeWindow() const override { return m_WindowHandle; }
        VkSurfaceKHR CreateVulkanSurface(VkInstance instance) const override;

    private:
        void Init(const WindowProps& props);
        void Shutdown();

    private:
        Display* m_Display;
        Window m_WindowHandle;

        struct WindowData 
        {
            std::string Title;
            int Width, Height;
        };

        WindowData m_Data;
    };

} // namespace Onyx

#endif
