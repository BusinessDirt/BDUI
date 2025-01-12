#pragma once

#include "Mixture/Core/Base.hpp"

#include <vulkan/vulkan.h>

namespace Mixture
{
	class DrawCommand
	{
	public:
		static void Draw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount = 1, uint32_t firstVertex = 0, uint32_t firstInstance = 0);
		static void DrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount = 1, uint32_t firstIndex = 0,
			int32_t vertexOffset = 0, uint32_t firstInstance = 0);
	};
}