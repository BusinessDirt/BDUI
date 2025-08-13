#pragma once

#include "Mixture/Core/Base.hpp"
#include "Mixture/Renderer/RendererInfo.hpp"
#include "Platform/Vulkan/Buffer/Buffer.hpp"

#include <vulkan/vulkan.h>

namespace Mixture
{
	class DrawCommand
	{
	public:
		static void Draw(FrameInfo& frameInfo, uint32_t vertexCount, uint32_t instanceCount = 1, uint32_t firstVertex = 0, uint32_t firstInstance = 0);
		static void DrawIndexed(FrameInfo& frameInfo, uint32_t indexCount, uint32_t instanceCount = 1, uint32_t firstIndex = 0,
			int32_t vertexOffset = 0, uint32_t firstInstance = 0);

		static void DrawIndirect(FrameInfo& frameInfo, const Vulkan::Buffer& buffer, VkDeviceSize offset);
		static void DrawIndexedIndirect(FrameInfo& frameInfo, const Vulkan::Buffer& buffer, VkDeviceSize offset);
	};
}
