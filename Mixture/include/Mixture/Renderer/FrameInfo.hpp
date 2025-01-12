#pragma once

#include "Mixture/Core/Base.hpp"

#include <vulkan/vulkan.h>

struct FrameInfo
{
	uint32_t FrameIndex;
	float FrameTime;
	VkCommandBuffer CommandBuffer;
};