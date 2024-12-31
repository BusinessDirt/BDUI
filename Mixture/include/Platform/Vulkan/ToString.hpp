#pragma once

#include <vulkan/vulkan.h>

#include <string>

namespace Vulkan::ToString
{

    std::string Format(VkFormat format);
    std::string ColorSpace(VkColorSpaceKHR colorSpace);
    std::string PresentMode(VkPresentModeKHR presentMode);
    std::string PhysicalDeviceType(VkPhysicalDeviceType physicalDeviceType);
    std::string Version(uint32_t version);
}
