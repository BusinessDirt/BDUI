#pragma once

#include "Platform/Vulkan/Base.hpp"
#include "Platform/Vulkan/Instance.hpp"

#include <Opal/Base.hpp>

namespace Vulkan
{
    class DebugMessenger
    {
    public:
        DebugMessenger(const VkInstance instance);
        ~DebugMessenger();
        
        OPAL_NON_COPIABLE(DebugMessenger);

        static void PopulateCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    private:
        VULKAN_HANDLE(VkDebugUtilsMessengerEXT, m_DebugMessenger);
        
        const VkInstance m_Instance;
    };
}
