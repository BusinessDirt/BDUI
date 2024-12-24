#pragma once

#include "Platform/Vulkan/Base.hpp"

namespace Vulkan
{
    class Instance
    {
    public:
        Instance(const std::string& applicationName, const std::vector<const char*>& requiredLayers, const std::vector<const char*>& requiredExtensions);
        ~Instance();
        
        Instance(const Instance&) = delete;
        Instance& operator=(const Instance&) = delete;
        Instance(Instance&& other) noexcept;
        Instance& operator=(Instance&& other) noexcept;
        
    private:
        VULKAN_HANDLE(VkInstance, m_Instance);
    };
}
