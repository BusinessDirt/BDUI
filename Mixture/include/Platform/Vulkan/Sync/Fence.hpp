#pragma once

#include "Platform/Vulkan/Base.hpp"

namespace Mixture::Vulkan
{
    class Fence
    {
    public:
        Fence(const Fence&) = delete;
        Fence& operator=(const Fence&) = delete;

        Fence(Fence&& other);
        Fence& operator=(Fence&& other);

        Fence(bool signaled);
        ~Fence();

        void Wait(uint64_t timeout);
        void Reset();

    private:
        VULKAN_HANDLE(VkFence, m_Fence);
    };
}