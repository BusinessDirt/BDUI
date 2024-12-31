#pragma once

#include "Mixture/Core/Base.hpp"

#include <Opal/Base.hpp>

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <cstring>

#define VK_ASSERT(arg, err) VkResult res = arg; OPAL_CORE_ASSERT(res == VK_SUCCESS, err)

#define VULKAN_HANDLE(VulkanHandleType, name) \
    public: VulkanHandleType GetHandle() const { return name; } \
    private: VulkanHandleType name = VK_NULL_HANDLE

// XCode doesn't support ansi colors in its terminal
#ifdef OPAL_PLATFORM_DARWIN
    #define ANSI_GREEN std::string("")
    #define ANSI_YELLOW std::string("")
    #define ANSI_RED std::string("")
    #define ANSI_RESET std::string("")
#else
    #define ANSI_GREEN std::string("\033[32m")
    #define ANSI_YELLOW std::string("\033[33m")
    #define ANSI_RED std::string("\033[31m")
    #define ANSI_RESET std::string("\033[0m")
#endif

#define AVAILABLE_AND_REQUIRED ANSI_GREEN + " ✓ "
#define AVAILABLE_NOT_REQUIRED ANSI_YELLOW + " - "
#define NOT_AVAILABLE_REQUIRED ANSI_RED + " ˟ "

#define VULKAN_INFO_HORIZONTAL_BAR "=========================="
#define VULKAN_INFO_BEGIN(title) OPAL_CORE_INFO(""); OPAL_CORE_INFO(std::string(title) + ":"); OPAL_CORE_INFO(VULKAN_INFO_HORIZONTAL_BAR)
#define VULKAN_INFO_END() OPAL_CORE_INFO(VULKAN_INFO_HORIZONTAL_BAR)
