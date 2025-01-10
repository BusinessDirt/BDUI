#pragma once

#include "Platform/Vulkan/Base.hpp"

namespace Mixture::Vulkan
{
    class Instance
    {
    public:
        Instance(const std::string& applicationName, const std::vector<const char*>& requiredLayers, const std::vector<const char*>& requiredExtensions);
        ~Instance();
        
        OPAL_NON_COPIABLE(Instance);
        
    private:
        VULKAN_HANDLE(VkInstance, m_Instance);
    };
}
