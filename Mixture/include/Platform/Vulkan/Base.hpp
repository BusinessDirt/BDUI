#pragma once

#include "Mixture/Core/Base.hpp"

#include <Opal/Base.hpp>

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <cstring>

#include "Mixture/Util/ToString.hpp"

#define VK_ASSERT(arg, err) { VkResult res = arg; OPAL_CORE_ASSERT(res == VK_SUCCESS, err); }

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

#define LIST_ITEM_CHECK ANSI_GREEN + " [✓] "
#define LIST_ITEM ANSI_YELLOW + " [-] "
#define LIST_ITEM_CROSS ANSI_RED + " [˟] "
#define LIST_ITEM_BLANK " [-] "

#define VULKAN_INFO_HORIZONTAL_BAR "=========================="
#define VULKAN_INFO_BEGIN(title) OPAL_CORE_INFO(""); OPAL_CORE_INFO(std::string(title) + ":"); OPAL_CORE_INFO(VULKAN_INFO_HORIZONTAL_BAR)
#define VULKAN_INFO_LIST(text, tabs, ...) OPAL_CORE_INFO(fmt::runtime(std::string(tabs * 2, ' ') + std::string(LIST_ITEM_BLANK) + text), __VA_ARGS__)
#define VULKAN_INFO_LIST_HEADER(text, tabs) OPAL_CORE_INFO(fmt::runtime(std::string(tabs * 2, ' ') + std::string(LIST_ITEM_BLANK) + text))
#define VULKAN_INFO_END() OPAL_CORE_INFO(VULKAN_INFO_HORIZONTAL_BAR)

namespace Mixture::Vulkan
{
    namespace Util
    {
        static bool IsRequired(const char* name, const std::vector<const char*>& required)
        {
            for (const auto& prop : required)
            {
                if (strcmp(name, prop) == 0) return true;
            }
            
            return false;
        }

        template <typename T, typename GetNameFunc>
        void PrintDebugAvailability(const std::vector<T>& availableItems, const std::vector<const char*>& requiredItems, GetNameFunc getName, const char* infoTitle)
        {
            VULKAN_INFO_BEGIN(infoTitle);

            // Print available items
            for (const auto& item : availableItems)
            {
                const char* itemName = getName(item);
                OPAL_CORE_INFO("{}{}{}", IsRequired(itemName, requiredItems) ? LIST_ITEM_CHECK : LIST_ITEM, itemName, ANSI_RESET);
            }

            // Check for required items that are not available
            for (const auto& requiredItem : requiredItems)
            {
                bool isAvailable = std::any_of(availableItems.begin(), availableItems.end(),
                    [&requiredItem, &getName](const T& item)
                    {
                        return strcmp(getName(item), requiredItem) == 0;
                    });

                if (!isAvailable) OPAL_CORE_INFO("{}{}{}", LIST_ITEM_CROSS, requiredItem, ANSI_RESET);
            }

            VULKAN_INFO_END();
        }
    }
}
