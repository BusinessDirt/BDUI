#pragma once

#include "Mixture/Core/Base.hpp"
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

        WindowProps(const std::string& title = "Onyx Window", int width = 800, int height = 600)
            : Title(title), Width(width), Height(height) {}
    };

    class Window 
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        Window(const WindowProps& props);
        ~Window();

        unsigned int GetWidth() const { return m_Data.Width; }
        unsigned int GetHeight() const { return m_Data.Height; }
        float GetAspectRatio() const { return m_Data.AspectRatio; }
        void* GetNativeWindow() const { return m_WindowHandle; }

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

