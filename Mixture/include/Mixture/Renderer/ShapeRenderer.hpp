#pragma once

#include "Mixture/Core/Base.hpp"
#include "Mixture/Renderer/RendererInfo.hpp"

#include "Platform/Vulkan/GraphicsPipeline.hpp"
#include "Platform/Vulkan/Buffer/Vertex.hpp"
#include "Platform/Vulkan/Buffer/Buffer.hpp"

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

namespace Mixture
{
	class ShapeRenderer
	{
	public:
		ShapeRenderer() = default;
		~ShapeRenderer() = default;

		static void Initialize();
		static void Shutdown();

		static void DrawRectangle(const glm::vec2& begin, const glm::vec2& end, const glm::vec3& color);
		static void Render(const FrameInfo& frameInfo);

	private:
		static Scope<Vulkan::GraphicsPipeline> s_Pipeline;
		static Scope<Vulkan::VertexBuffer> s_DynamicVertexBuffer;
		static Vector<Vertex> s_Vertices;
		static size_t s_PreviousHash;
	};
}