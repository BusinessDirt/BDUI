#pragma once

#include "Platform/Vulkan/Base.hpp"
#include "Platform/Vulkan/Instance.hpp"
#include "Platform/Vulkan/DebugMessenger.hpp"

namespace Vulkan
{
    class Context
    {
    public:
        Context() = default;
        ~Context();
        
        void Initialize(const std::string& applicationName);
        
    private:
        std::unique_ptr<Instance> m_Instance = nullptr;
        std::unique_ptr<DebugMessenger> m_DebugMessenger = nullptr;
    };
}
