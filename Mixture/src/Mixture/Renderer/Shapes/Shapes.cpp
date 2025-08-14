#include "mxpch.hpp"
#include "Mixture/Renderer/Shapes/Shapes.hpp"

#include "Mixture/Util/Util.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace Mixture
{
    Rectangle::Rectangle(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color)
        : m_Start(start), m_End(end), m_Color(color)
    {}

    void Rectangle::Tesselate(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) const
    {
        uint32_t startIndex = static_cast<uint32_t>(vertices.size());

        vertices.push_back({ { m_Start.x, m_Start.y, 0.0f }, m_Color });
        vertices.push_back({ { m_End.x,   m_Start.y, 0.0f }, m_Color });
        vertices.push_back({ { m_End.x,   m_End.y,   0.0f }, m_Color });
        vertices.push_back({ { m_Start.x, m_End.y,   0.0f }, m_Color });

        indices.insert(indices.end(), {
            startIndex, startIndex+1, startIndex+2,
            startIndex, startIndex+2, startIndex+3
        });
    }

    void Rectangle::HashCombine(std::size_t& seed) const
    {
        Util::HashCombine(seed, m_Start, m_End, m_Color);
    }

    Line::Line(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, float thickness)
        : m_Start(start), m_End(end), m_Color(color), m_Thickness(thickness)
    {}

    void Line::Tesselate(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) const
    {
        // Create a perpendicular offset for line thickness
        glm::vec3 dir = m_End - m_Start;
        glm::vec2 perp = glm::normalize(glm::vec2(-dir.y, dir.x)) * (m_Thickness * 0.5f);
        glm::vec3 perp3D = glm::vec3(perp.x, perp.y, 0.0f);

        glm::vec3 v0 = m_Start + perp3D;
        glm::vec3 v1 = m_Start - perp3D;
        glm::vec3 v2 = m_End   - perp3D;
        glm::vec3 v3 = m_End   + perp3D;

        uint32_t startIndex = static_cast<uint32_t>(vertices.size());
        
        vertices.push_back({ v0, m_Color });
        vertices.push_back({ v1, m_Color });
        vertices.push_back({ v2, m_Color });
        vertices.push_back({ v3, m_Color });
        
        indices.insert(indices.end(), {
            startIndex, startIndex+1, startIndex+2,
            startIndex, startIndex+2, startIndex+3
        });
    }

    void Line::HashCombine(std::size_t& seed) const
    {
        Util::HashCombine(seed, m_Start, m_End, m_Color, m_Thickness);
    }
}
