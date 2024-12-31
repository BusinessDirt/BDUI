#pragma once
#include <Opal/Base.hpp>
#ifdef OPAL_PLATFORM_DARWIN

#include "Mixture/Core/Window.hpp"

namespace Mixture
{

    class DarwinWindow : public Window
    {
    public:
        DarwinWindow(const WindowProps& props);
        ~DarwinWindow();

        void OnUpdate() override;
        int GetWidth() const override { return m_Data.Width; }
        int GetHeight() const override { return m_Data.Height; }
        
        void* GetNativeWindow() const override { return m_WindowHandle; }
        VkSurfaceKHR CreateVulkanSurface(VkInstance instance) const override;
        
        void OnResize(int width, int height);
        void OnClose();

    private:
        void OnEvent(void* e);
        void Init(const WindowProps& props);
        void Shutdown();

    private:
        void* m_WindowHandle;
        void* m_NSView;
        void* m_MetalLayer;

        struct WindowData
        {
            std::string Title;
            int Width, Height;
        };

        WindowData m_Data;
    };

} // namespace Onyx

#endif
