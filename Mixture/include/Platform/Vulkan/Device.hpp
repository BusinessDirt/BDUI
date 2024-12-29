#pragma once

#include "Platform/Vulkan/Base.hpp"

#include <Opal/Base.hpp>

namespace Vulkan
{
    class PhysicalDevice;
    
    class Device
    {
    public:
        Device(const PhysicalDevice& physicalDevice, const std::vector<const char*>& requiredLayers, const std::vector<const char*>& requiredExtensions);
        ~Device();
        
        OPAL_NON_COPIABLE(Device);
        
        const VkQueue& GetGraphicsQueue() const { return m_GraphicsQueue; }
        
    private:
        VULKAN_HANDLE(VkDevice, m_Device);
        VkQueue m_GraphicsQueue;
        
        const PhysicalDevice& m_PhysicalDevice;
    };
}
