#pragma once

#include <vulkan/vulkan.h>

#include <string>

namespace Mixture::Vulkan::ToString
{

    std::string_view Format(VkFormat format);
    std::string_view ShaderFormat(VkFormat format);
    std::string_view ColorSpace(VkColorSpaceKHR colorSpace);
    std::string_view PresentMode(VkPresentModeKHR presentMode);
    std::string_view PhysicalDeviceType(VkPhysicalDeviceType physicalDeviceType);
    std::string_view DescriptorType(VkDescriptorType descriptorType);
    std::string Version(uint32_t version);
}
