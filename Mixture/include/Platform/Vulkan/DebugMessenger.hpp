#pragma once

#include "Platform/Vulkan/Base.hpp"

#include <Opal/Base.hpp>

namespace Mixture::Vulkan
{
    class DebugMessenger
    {
    public:
        DebugMessenger();
        ~DebugMessenger();
        
        OPAL_NON_COPIABLE(DebugMessenger);

        static void PopulateCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    private:
        VULKAN_HANDLE(VkDebugUtilsMessengerEXT, m_DebugMessenger);
    };
}
