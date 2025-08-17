#pragma once

#include "Platform/Vulkan/Base.hpp"

namespace Mixture::Vulkan
{
    class Semaphore
    {
    public:
        Semaphore(const Semaphore&) = delete;
        Semaphore& operator=(const Semaphore&) = delete;

        Semaphore(Semaphore&& other) noexcept;
        Semaphore& operator=(Semaphore&& other) noexcept;

        Semaphore();
        ~Semaphore();

    private:
        VULKAN_HANDLE(VkSemaphore, m_Semaphore);
    };
}