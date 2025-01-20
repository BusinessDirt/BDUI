#include "mxpch.hpp"
#include "Mixture/Renderer/ShapeRenderer.hpp"

#include "Mixture/Util/VulkanUtil.hpp"
#include "Mixture/Renderer/DrawCommand.hpp"

#include <functional>

namespace Mixture
{
	Scope<Vulkan::GraphicsPipeline> ShapeRenderer::s_Pipeline = nullptr;
	Scope<Vulkan::VertexBuffer> ShapeRenderer::s_DynamicVertexBuffer = nullptr;
	Vector<Vertex> ShapeRenderer::s_Vertices = Vector<Vertex>();
	size_t ShapeRenderer::s_PreviousHash = 0;

	namespace Util
	{
		static size_t Fnv1aHash(const void* data, size_t size) 
		{
			const uint8_t* byteData = static_cast<const uint8_t*>(data);
			constexpr const size_t magicPrime = 0x00000100000001b3;
			size_t hash                       = 0xcbf29ce484222325;

			for (size_t i = 0; i < size; ++i) 
				hash = (hash ^ byteData[i]) * magicPrime;

			return hash;
		}
	}

	void ShapeRenderer::Initialize()
	{
		s_Pipeline = CreateGraphicsPipeline("shader");
		s_DynamicVertexBuffer = CreateVertexBuffer();
	}

	void ShapeRenderer::Shutdown()
	{
		s_DynamicVertexBuffer.reset();
		s_Pipeline.reset();
	}

	void ShapeRenderer::DrawRectangle(const glm::vec2& begin, const glm::vec2& end, const glm::vec3& color)
	{
		s_Vertices.insert(s_Vertices.begin(), { { begin.x, begin.y, 0.0f }, color });
		s_Vertices.insert(s_Vertices.begin(), { { begin.x, end.y,   0.0f }, color });
		s_Vertices.insert(s_Vertices.begin(), { { end.x,   end.y,   0.0f }, color });
		s_Vertices.insert(s_Vertices.begin(), { { end.x,   end.y,   0.0f }, color });
		s_Vertices.insert(s_Vertices.begin(), { { end.x,   begin.y, 0.0f }, color });
		s_Vertices.insert(s_Vertices.begin(), { { begin.x, begin.y, 0.0f }, color });
	}

	void ShapeRenderer::Render(const FrameInfo& frameInfo)
	{
		if (s_Vertices.empty()) return;

		// check if we need to update the vertex buffer
		size_t hash = Util::Fnv1aHash(s_Vertices.data(), s_Vertices.size() * sizeof(Vertex));
		if (hash != s_PreviousHash)
		{
			s_DynamicVertexBuffer->SetData(s_Vertices);
			s_PreviousHash = hash;
		}

		s_Pipeline->Bind(frameInfo);
		s_DynamicVertexBuffer->Bind(frameInfo.CommandBuffer);
		DrawCommand::Draw(frameInfo.CommandBuffer, s_DynamicVertexBuffer->GetVertexCount());

		s_Vertices.clear();
	}
}