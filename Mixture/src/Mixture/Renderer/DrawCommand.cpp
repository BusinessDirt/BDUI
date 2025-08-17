#include "mxpch.hpp"
#include "Mixture/Renderer/DrawCommand.hpp"

namespace Mixture 
{
	void DrawCommand::Draw(const VkCommandBuffer commandBuffer, const uint32_t vertexCount,
						   const uint32_t instanceCount, const uint32_t firstVertex, const uint32_t firstInstance)
	{
		vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void DrawCommand::DrawIndexed(const VkCommandBuffer commandBuffer, const uint32_t indexCount, const uint32_t instanceCount,
								  const uint32_t firstIndex, const int32_t vertexOffset, const uint32_t firstInstance)
	{
		vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}

	void DrawCommand::DrawIndirect(const VkCommandBuffer commandBuffer, const Vulkan::Buffer& buffer, const VkDeviceSize offset)
	{
		vkCmdDrawIndirect(commandBuffer, buffer.GetHandle(), offset, buffer.GetInstanceCount(), static_cast<uint32_t>(buffer.GetInstanceSize()));
	}

	void DrawCommand::DrawIndexedIndirect(const VkCommandBuffer commandBuffer, const Vulkan::Buffer& buffer, const VkDeviceSize offset)
	{
		vkCmdDrawIndexedIndirect(commandBuffer, buffer.GetHandle(), offset, buffer.GetInstanceCount(), static_cast<uint32_t>(buffer.GetInstanceSize()));
	}
}

