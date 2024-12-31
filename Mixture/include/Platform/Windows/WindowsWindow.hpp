#pragma once
#include <Opal/Base.hpp>
#ifdef OPAL_PLATFORM_WINDOWS

#include "Mixture/Core/Window.hpp"

#include <Windows.h>

namespace Mixture {

    class WindowsWindow : public Window {
    public:
        WindowsWindow(const WindowProps& props);
        ~WindowsWindow();

        void OnUpdate() override;
        int GetWidth() const override { return m_Data.Width; }
        int GetHeight() const override { return m_Data.Height; }
        
        void* GetNativeWindow() const override { return m_WindowHandle; }
        VkSurfaceKHR CreateVulkanSurface(VkInstance instance) const override;

    private:
        void Init(const WindowProps& props);
        void Shutdown();

    private:
        HWND m_WindowHandle;

        struct WindowData 
        {
            std::string Title;
            int Width, Height;
        };

        WindowData m_Data;
    };

} // namespace Onyx

#endif
