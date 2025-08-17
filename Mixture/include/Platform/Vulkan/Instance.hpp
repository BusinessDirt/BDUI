#pragma once

#include "Platform/Vulkan/Base.hpp"

namespace Mixture::Vulkan
{
    class Instance
    {
    public:
        OPAL_NON_COPIABLE(Instance);
        
        Instance(const std::string& applicationName, const std::vector<const char*>& requiredLayers, const std::vector<const char*>& requiredExtensions);
        ~Instance();
        
    private:
        VULKAN_HANDLE(VkInstance, m_Instance);
    };
}
