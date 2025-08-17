#pragma once

#include "Platform/Vulkan/Base.hpp"

namespace Mixture::Vulkan
{
    class Fence
    {
    public:
        Fence(const Fence&) = delete;
        Fence& operator=(const Fence&) = delete;

        Fence(Fence&& other) noexcept;
        Fence& operator=(Fence&& other) noexcept;

        explicit Fence(bool signaled);
        ~Fence();

        void Wait(uint64_t timeout) const;
        void Reset() const;

    private:
        VULKAN_HANDLE(VkFence, m_Fence);
    };
}