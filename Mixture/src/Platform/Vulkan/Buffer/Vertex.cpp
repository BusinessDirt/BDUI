#include "mxpch.hpp"
#include "Platform/Vulkan/Buffer/Vertex.hpp"
namespace Mixture::Vulkan
{
    VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices)
    {
        SetData(vertices);
    }

    VertexBuffer::VertexBuffer()
        : m_VertexCount(0), m_VertexBuffer(VK_NULL_HANDLE)
    {}

    VertexBuffer::~VertexBuffer()
    {
        m_VertexBuffer = nullptr;
    }

    void VertexBuffer::SetData(const std::vector<Vertex>& vertices)
    {
        m_VertexCount = static_cast<uint32_t>(vertices.size());
        OPAL_CORE_ASSERT(m_VertexCount >= 3, "Vertex count must be at least 3!");

        uint32_t vertexSize = sizeof(Vertex);
        VkDeviceSize bufferSize = vertexSize * m_VertexCount;

        // Create a staging buffer for data transfer
        Buffer stagingBuffer = Buffer(vertexSize, m_VertexCount, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        // Map and write data to the staging buffer
        stagingBuffer.Map();
        stagingBuffer.WriteToBuffer((void*)vertices.data());

        // If the vertex buffer hasn't been created yet, create it
        if (!m_VertexBuffer || m_VertexBuffer->GetBufferSize() < bufferSize)
        {
            m_VertexBuffer = CreateScope<Buffer>(vertexSize, m_VertexCount, 
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        }

        // Copy data from the staging buffer to the GPU vertex buffer
        Buffer::Copy(stagingBuffer.GetHandle(), m_VertexBuffer->GetHandle(), bufferSize);
    }

    void VertexBuffer::Bind(FrameInfo& frameInfo)
    {
        VkBuffer buffers[] = { m_VertexBuffer->GetHandle() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(frameInfo.CommandBuffer, 0, 1, buffers, offsets);
    }
}
