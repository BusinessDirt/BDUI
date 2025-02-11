#include "mxpch.hpp"
#include "Mixture/Renderer/DrawCommand.hpp"

namespace Mixture 
{
	void DrawCommand::Draw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
	{
		vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void DrawCommand::DrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
	{
		vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}

	void DrawCommand::DrawIndirect(VkCommandBuffer commandBuffer, const Vulkan::Buffer& buffer, VkDeviceSize offset)
	{
		vkCmdDrawIndirect(commandBuffer, buffer.GetHandle(), offset, buffer.GetInstanceCount(), buffer.GetInstanceSize());
	}

	void DrawCommand::DrawIndexedIndirect(VkCommandBuffer commandBuffer, const Vulkan::Buffer& buffer, VkDeviceSize offset)
	{
		vkCmdDrawIndexedIndirect(commandBuffer, buffer.GetHandle(), offset, buffer.GetInstanceCount(), buffer.GetInstanceSize());
	}
}

