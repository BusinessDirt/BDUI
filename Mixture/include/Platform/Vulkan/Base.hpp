#pragma once

#include "Mixture/Core/Base.hpp"

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <cstring>

#define VK_ASSERT(arg, err) VkResult res = arg; OPAL_CORE_ASSERT(res == VK_SUCCESS, err)

#define VULKAN_HANDLE(VulkanHandleType, name) \
    public: VulkanHandleType GetHandle() const { return name; } \
    private: VulkanHandleType name = VK_NULL_HANDLE
