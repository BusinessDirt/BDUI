#include "mxpch.hpp"
#include "Platform/Vulkan/Sync/Fence.hpp"

#include "Platform/Vulkan/Context.hpp"

namespace Mixture::Vulkan
{
    Fence::Fence(const bool signaled)
    {
        VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

        VK_ASSERT(vkCreateFence(Context::Get().Device().GetHandle(), &fenceInfo, nullptr, &m_Fence),
                  "Failed to create VkFence!")
    }

    Fence::~Fence()
    {
        if (m_Fence)
        {
            vkDestroyFence(Context::Get().Device().GetHandle(), m_Fence, nullptr);
            m_Fence = nullptr;
        }
    }

    Fence::Fence(Fence&& other) noexcept
        : m_Fence(other.m_Fence)
    {
        other.m_Fence = VK_NULL_HANDLE;
    }

    Fence& Fence::operator=(Fence&& other) noexcept
    {
        if (this != &other)
        {
            this->~Fence();
            m_Fence = other.m_Fence;
            other.m_Fence = VK_NULL_HANDLE;
        }
        return *this;
    }

    void Fence::Wait(const uint64_t timeout) const
    {
        vkWaitForFences(Context::Get().Device().GetHandle(), 1, &m_Fence, VK_TRUE, timeout);
    }

    void Fence::Reset() const
    {
        vkResetFences(Context::Get().Device().GetHandle(), 1, &m_Fence);
    }
}
