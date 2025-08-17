#pragma once

#include "Mixture/Events/Event.hpp"

#include <string>
#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace Mixture 
{
    struct WindowProps
    {
        std::string Title;
        int Width;
        int Height;

        explicit WindowProps(std::string title = "Mixture Window", const int width = 800, const int height = 600)
            : Title(std::move(title)), Width(width), Height(height) {}
    };

    class Window 
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        OPAL_NON_COPIABLE(Window);
        
        explicit Window(const WindowProps& props);
        ~Window();

        OPAL_NODISCARD unsigned int GetWidth() const { return m_Data.Width; }
        OPAL_NODISCARD unsigned int GetHeight() const { return m_Data.Height; }
        OPAL_NODISCARD float GetAspectRatio() const { return m_Data.AspectRatio; }
        OPAL_NODISCARD void* GetNativeWindow() const { return m_WindowHandle; }

        void OnUpdate() const;
        void GetFramebufferSize(int* width, int* height) const;
        void CreateVulkanSurface(VkInstance instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface) const;
        
        // Event callback setter
        void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }

    private:
        struct WindowData
        {
            std::string Title;
            unsigned int Width = 0, Height = 0;
            float AspectRatio = 0.0f;
            bool VSync = true;
            bool Minimized = false;
            EventCallbackFn EventCallback;
        };

    private:
        GLFWwindow* m_WindowHandle;
        WindowData m_Data;
        std::function<void(Event&)> m_EventCallback;
    };
}

