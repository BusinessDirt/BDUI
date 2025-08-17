#pragma once

#include "Platform/Vulkan/Base.hpp"

#include <Opal/Base.hpp>

namespace Mixture::Vulkan
{
    class PhysicalDevice;
    
    class Device
    {
    public:
        Device(const std::vector<const char*>& requiredLayers, const std::vector<const char*>& requiredExtensions);
        ~Device();
        
        OPAL_NON_COPIABLE(Device);

        static VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void CreateImageWithInfo(const VkImageCreateInfo& imageInfo, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) const;
        
        OPAL_NODISCARD VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }
        OPAL_NODISCARD VkQueue GetPresentQueue() const { return m_PresentQueue; }
        
    private:
        VULKAN_HANDLE(VkDevice, m_Device);
        
        VkQueue m_GraphicsQueue{ VK_NULL_HANDLE };
        VkQueue m_PresentQueue{ VK_NULL_HANDLE };
    };
}
