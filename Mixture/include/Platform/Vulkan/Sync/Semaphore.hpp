#pragma once

#include "Platform/Vulkan/Base.hpp"

namespace Vulkan
{
    class Semaphore
    {
    public:
        Semaphore(const Semaphore&) = delete;
        Semaphore& operator=(const Semaphore&) = delete;

        Semaphore(Semaphore&& other);
        Semaphore& operator=(Semaphore&& other);

        Semaphore();
        ~Semaphore();

    private:
        VULKAN_HANDLE(VkSemaphore, m_Semaphore);
    };
}