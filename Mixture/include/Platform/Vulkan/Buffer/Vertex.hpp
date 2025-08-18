#pragma once

#include "Mixture/Renderer/RendererInfo.hpp"

#include "Platform/Vulkan/Base.hpp"
#include "Platform/Vulkan/Buffer/Buffer.hpp"

namespace Mixture::Vulkan
{
    template<class T>
    class VertexBuffer
    {
    public:
        OPAL_NON_COPIABLE(VertexBuffer);

        explicit VertexBuffer(const std::vector<T>& vertices = {});
        ~VertexBuffer();

        void SetData(const std::vector<T>& vertices, VkCommandBuffer commandBuffer = VK_NULL_HANDLE);
            
        void Bind(VkCommandBuffer commandBuffer) const;
            
        OPAL_NODISCARD uint32_t GetVertexCount() const { return m_VertexCount; }

    private:
        uint32_t m_VertexCount = 0;
        Scope<Buffer> m_VertexBuffer;
    };

    template<class T>
    inline VertexBuffer<T>::VertexBuffer(const std::vector<T>& vertices)
        : m_VertexBuffer(VK_NULL_HANDLE)
    {
        if (!vertices.empty()) SetData(vertices, VK_NULL_HANDLE);
    }

    template<class T>
    inline VertexBuffer<T>::~VertexBuffer()
    {
        m_VertexBuffer = nullptr;
    }

    template<class T>
    inline void VertexBuffer<T>::SetData(const std::vector<T>& vertices, const VkCommandBuffer commandBuffer)
    {
        uint64_t vertexSize = static_cast<uint64_t>(sizeof(T));
        m_VertexCount = static_cast<uint32_t>(vertices.size());
        OPAL_CORE_ASSERT(m_VertexCount >= 3, "Vertex count must be at least 3!")

        const VkDeviceSize bufferSize = vertexSize * m_VertexCount;

        // Create a staging buffer for data transfer
        auto stagingBuffer = Buffer(vertexSize, m_VertexCount, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        // Map and write data to the staging buffer
        stagingBuffer.Map();
        stagingBuffer.WriteToBuffer(vertices.data());

        // If the vertex buffer hasn't been created yet, create it
        if (!m_VertexBuffer || m_VertexBuffer->GetBufferSize() < bufferSize)
        {
            m_VertexBuffer = CreateScope<Buffer>(vertexSize, m_VertexCount,
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        }

        // Copy data from the staging buffer to the GPU vertex buffer
        Buffer::Copy(stagingBuffer.GetHandle(), m_VertexBuffer->GetHandle(), bufferSize, commandBuffer);
    }


    template<class T>
    inline void VertexBuffer<T>::Bind(const VkCommandBuffer commandBuffer) const
    {
        const VkBuffer buffers[] = { m_VertexBuffer->GetHandle() };
        constexpr VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    }
}

