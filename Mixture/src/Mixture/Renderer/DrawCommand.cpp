#include "mxpch.hpp"
#include "Mixture/Renderer/DrawCommand.hpp"

namespace Mixture 
{
	void DrawCommand::Draw(FrameInfo& frameInfo, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
	{
		vkCmdDraw(frameInfo.CommandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
#ifndef OPAL_DIST
        frameInfo.DrawCalls += 1;
        frameInfo.TriangleCount += vertexCount / 3;
#endif
	}

	void DrawCommand::DrawIndexed(FrameInfo& frameInfo, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
	{
		vkCmdDrawIndexed(frameInfo.CommandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
#ifndef OPAL_DIST
        frameInfo.DrawCalls += 1;
        frameInfo.TriangleCount += indexCount / 3;
#endif
	}

	void DrawCommand::DrawIndirect(FrameInfo& frameInfo, const Vulkan::Buffer& buffer, VkDeviceSize offset)
	{
		vkCmdDrawIndirect(frameInfo.CommandBuffer, buffer.GetHandle(), offset, buffer.GetInstanceCount(), static_cast<uint32_t>(buffer.GetInstanceSize()));
#ifndef OPAL_DIST
        frameInfo.DrawCalls += 1;
#endif
	}

	void DrawCommand::DrawIndexedIndirect(FrameInfo& frameInfo, const Vulkan::Buffer& buffer, VkDeviceSize offset)
	{
		vkCmdDrawIndexedIndirect(frameInfo.CommandBuffer, buffer.GetHandle(), offset, buffer.GetInstanceCount(), static_cast<uint32_t>(buffer.GetInstanceSize()));
#ifndef OPAL_DIST
        frameInfo.DrawCalls += 1;
#endif
	}
}

