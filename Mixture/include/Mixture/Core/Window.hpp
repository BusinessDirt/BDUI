#pragma once

#include "Mixture/Core/Base.hpp"
#include "Mixture/Events/Event.hpp"

#include <string>
#include <vulkan/vulkan.h>

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
        virtual ~Window() = default;

        // Pure virtual methods to be implemented by platform-specific classes
        virtual void OnUpdate() = 0;               // Called each frame to process events and update the window
        virtual int GetWidth() const = 0;         // Returns the width of the window
        virtual int GetHeight() const = 0;        // Returns the height of the window
        
        virtual void* GetNativeWindow() const = 0; // Returns a platform-specific window handle
        virtual VkSurfaceKHR CreateVulkanSurface(VkInstance instance) const = 0;
        
        // Event callback setter
        void SetEventCallback(const std::function<void(Event&)>& callback);

        // Factory method to create platform-specific windows
        static Scope<Window> Create(const WindowProps& props = WindowProps());
    protected:
        std::function<void(Event&)> m_EventCallback;
    };
}

