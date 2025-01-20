#pragma once

#include "Mixture/Core/Base.hpp"

#include "Platform/Vulkan/Buffer/Index.hpp"
#include "Platform/Vulkan/Buffer/Vertex.hpp"
#include "Platform/Vulkan/Buffer/Buffer.hpp"
#include "Platform/Vulkan/GraphicsPipeline.hpp"

#define CREATE_FUNC(type) template <typename... Args> static Scope<Vulkan::type> Create##type(Args&&... args) { return CreateScope<Vulkan::type>(std::forward<Args>(args)...); }

namespace Mixture
{
    CREATE_FUNC(IndexBuffer)
    CREATE_FUNC(VertexBuffer)
    CREATE_FUNC(Buffer)
    CREATE_FUNC(GraphicsPipeline)
}

#undef CREATE_FUNC
