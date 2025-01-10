#pragma once

#include "Mixture/Events/ApplicationEvent.hpp"

#include "Platform/Vulkan/Base.hpp"
#include "Platform/Vulkan/Instance.hpp"
#include "Platform/Vulkan/DebugMessenger.hpp"
#include "Platform/Vulkan/WindowSurface.hpp"
#include "Platform/Vulkan/PhysicalDevice.hpp"
#include "Platform/Vulkan/Device.hpp"
#include "Platform/Vulkan/Swapchain.hpp"
#include "Platform/Vulkan/GraphicsPipeline.hpp"

namespace Vulkan
{
    class Context
    {
    public:
        Context() = default;
        ~Context();
        
        void Initialize(const std::string& applicationName);
        bool OnWindowResize(Mixture::WindowResizeEvent& event);
        
    private:
        std::unique_ptr<Instance> m_Instance = nullptr;
        std::unique_ptr<DebugMessenger> m_DebugMessenger = nullptr;
        std::unique_ptr<WindowSurface> m_WindowSurface = nullptr;
        std::unique_ptr<PhysicalDevice> m_PhysicalDevice = nullptr;
        std::unique_ptr<Device> m_Device = nullptr;
        std::unique_ptr<Swapchain> m_Swapchain = nullptr;
        std::unique_ptr<GraphicsPipeline> m_GraphicsPipeline = nullptr;
    };
}
