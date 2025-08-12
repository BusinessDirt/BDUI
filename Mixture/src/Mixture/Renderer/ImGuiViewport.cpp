#include "Mixture/Renderer/ImGuiViewport.hpp"

#include "Platform/Vulkan/Context.hpp"

#include <imgui.h>

namespace Mixture
{
    void ImGuiViewport::Shutdown()
    {
        Cleanup();
    }

    void ImGuiViewport::BlitFromScene(VkCommandBuffer cmd, VkImage sceneImage, uint32_t sceneWidth, uint32_t sceneHeight)
    {
        TransitionImageLayout(cmd, sceneImage,
                              VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                              VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                              VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                              VK_PIPELINE_STAGE_TRANSFER_BIT);

        TransitionImageLayout(cmd, m_Image->GetHandle(),
                              VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                              VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                              VK_PIPELINE_STAGE_TRANSFER_BIT);

        VkImageBlit blit{};
        blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.layerCount = 1;
        blit.srcOffsets[1] = { static_cast<int32_t>(sceneWidth), static_cast<int32_t>(sceneHeight), 1 };

        blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.layerCount = 1;
        blit.dstOffsets[1] = { static_cast<int32_t>(m_ImageWidth), static_cast<int32_t>(m_ImageHeight), 1 };

        vkCmdBlitImage(cmd, sceneImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, m_Image->GetHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                       1, &blit, VK_FILTER_LINEAR);

        TransitionImageLayout(cmd, m_Image->GetHandle(),
                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                              VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                              VK_PIPELINE_STAGE_TRANSFER_BIT,
                              VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

        TransitionImageLayout(cmd, sceneImage,
                              VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                              VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                              VK_PIPELINE_STAGE_TRANSFER_BIT,
                              VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
    }

    void ImGuiViewport::Resize()
    {
        if (!m_Resize) return;
        m_Resize = false;
        
        m_ImageWidth = m_Width;
        m_ImageHeight = m_Height;
        
        Vulkan::Context::Get().WaitForDevice();
        Cleanup();
        CreateResources();
    }

    void ImGuiViewport::DrawWindow()
    {
        ImVec2 oldPadding = ImGui::GetStyle().WindowPadding;
        ImGui::GetStyle().WindowPadding = ImVec2(0.0f, 0.0f);
        ImGui::Begin("Viewport");
        ImVec2 size = ImGui::GetContentRegionAvail();
        
        if ((uint32_t)size.x != m_ImageWidth || (uint32_t)size.y != m_ImageHeight)
        {
            m_Resize = true;
            m_Width = (uint32_t)size.x;
            m_Height = (uint32_t)size.y;
        }
        
        ImGui::Image((ImTextureID)m_DescriptorSet, size, ImVec2(0,0), ImVec2(1,1));
        ImGui::End();
        ImGui::GetStyle().WindowPadding = oldPadding;
    }

    void ImGuiViewport::CreateResources()
    {
        VkExtent2D extent = { m_ImageWidth, m_ImageHeight };
        VkFormat format = Vulkan::Context::Get().Swapchain().GetImageFormat();
        
        m_Image = CreateScope<Vulkan::Image>(extent, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 1);
        m_ImageMemory = CreateScope<Vulkan::DeviceMemory>(m_Image->AllocateMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
        m_Image->TransitionImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        
        // Image View
        m_ImageView = CreateScope<Vulkan::ImageView>(m_Image->GetHandle(), format, VK_IMAGE_ASPECT_COLOR_BIT, 1);
        
        // Sampler
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.maxAnisotropy = 1.0f;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        VK_ASSERT(vkCreateSampler(Vulkan::Context::Get().Device().GetHandle(), &samplerInfo, nullptr, &m_ImageSampler),
                  "Failed to create Image Sampler");
        
        // Create descriptor set layout if needed:
        if (m_DescriptorSetLayout == VK_NULL_HANDLE)
        {
            VkDescriptorSetLayoutBinding layoutBinding{};
            layoutBinding.binding = 0;
            layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            layoutBinding.descriptorCount = 1;
            layoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

            VkDescriptorSetLayoutCreateInfo layoutInfo{};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = 1;
            layoutInfo.pBindings = &layoutBinding;

            VK_ASSERT(vkCreateDescriptorSetLayout(Vulkan::Context::Get().Device().GetHandle(), &layoutInfo, nullptr, &m_DescriptorSetLayout),
                      "Failed to create Descriptor Set Layout");
        }

        // Allocate descriptor set
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = Vulkan::Context::Get().DescriptorPool().GetHandle();
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &m_DescriptorSetLayout;

        VK_ASSERT(vkAllocateDescriptorSets(Vulkan::Context::Get().Device().GetHandle(), &allocInfo, &m_DescriptorSet),
                  "Failed to allocate Descriptor Set");

        // Update descriptor set
        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = m_ImageView->GetHandle();
        imageInfo.sampler = m_ImageSampler;

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = m_DescriptorSet;
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(Vulkan::Context::Get().Device().GetHandle(), 1, &descriptorWrite, 0, nullptr);
    }

    void ImGuiViewport::Cleanup()
    {
        if (m_DescriptorSetLayout != VK_NULL_HANDLE)
        {
            vkDestroyDescriptorSetLayout(Vulkan::Context::Get().Device().GetHandle(), m_DescriptorSetLayout, nullptr);
            m_DescriptorSetLayout = VK_NULL_HANDLE;
        }
            
        if (m_ImageSampler != VK_NULL_HANDLE)
            vkDestroySampler(Vulkan::Context::Get().Device().GetHandle(), m_ImageSampler, nullptr);
        m_ImageMemory.reset();
        m_ImageView.reset();
        m_Image.reset();
    }

    void ImGuiViewport::TransitionImageLayout(VkCommandBuffer cmd, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout,
            VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage)
    {
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        // Set access masks for common layouts
        if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        if (newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        else if (newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        else if (newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(cmd, srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    }
}
