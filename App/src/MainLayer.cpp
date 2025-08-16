#include "mxpch.hpp"
#include "MainLayer.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Mixture
{
    void MainLayer::OnAttach()
    {
        std::vector<Vertex> vertices = {
            { { -1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
            { {  1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
            { {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
            { { -1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }
        };
        
        std::vector<uint32_t> indices = {
            0, 1, 2, 0, 2, 3
        };
        
        m_GraphicsPipeline = CreateScope<Vulkan::GraphicsPipeline>("MixtureBuiltin.Object");
        m_VertexBuffer = CreateScope<Vulkan::VertexBuffer>(vertices);
        m_IndexBuffer = CreateScope<Vulkan::IndexBuffer>(indices);
        
        m_UniformBufferObject = {};
        m_UniformBuffer = CreateScope<Vulkan::Buffer>(sizeof(UniformBufferObject), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }

    void MainLayer::OnDetach()
    {
        m_GraphicsPipeline.reset();
        m_IndexBuffer.reset();
        m_VertexBuffer.reset();
    }

    void MainLayer::OnUpdate(FrameInfo& frameInfo)
    {
        m_UniformBufferObject.View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -m_Z));
        m_UniformBufferObject.Projection = glm::perspective(glm::radians(45.0f), Application::Get().GetWindow().GetAspectRatio(), 0.1f, 100.0f);
        
        m_UniformBuffer->Map();
        m_UniformBuffer->WriteToBuffer(&m_UniformBufferObject);
        m_UniformBuffer->Unmap();
        
        VkDescriptorBufferInfo uniformBufferInfo{};
        uniformBufferInfo.buffer = m_UniformBuffer->GetHandle();
        uniformBufferInfo.offset = 0;
        uniformBufferInfo.range  = sizeof(UniformBufferObject);
        m_GraphicsPipeline->UpdateGlobalUniformBuffer(0, &uniformBufferInfo);
    }

    void MainLayer::OnRender(FrameInfo& frameInfo)
    {
        m_GraphicsPipeline->Bind(frameInfo.CommandBuffer);
        m_VertexBuffer->Bind(frameInfo.CommandBuffer);
        m_IndexBuffer->Bind(frameInfo.CommandBuffer);
        
        DrawCommand::DrawIndexed(frameInfo.CommandBuffer, m_IndexBuffer->GetIndexCount());
    }

    void MainLayer::OnRenderImGui(FrameInfo& frameInfo)
    {
        ImGuiWidgets::Dockspace("MainDockspace", true, ImGuiWindowFlags_NoBackground, ImGuiDockNodeFlags_PassthruCentralNode);
        
        ImGui::Begin("Test");
        ImGui::Text("Frame time: %.3f ms", frameInfo.FrameTime);
        ImGui::Text("Viewport focused: %s", m_ViewportFocused ? "true" : "false");
        ImGui::SliderFloat("Distance", &m_Z, 0.1f, 100.0f);
        ImGui::End();
    }

    void MainLayer::OnEvent(Event& event)
    {
        
    }
}
