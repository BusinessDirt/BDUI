#pragma once

#include "Mixture/Core/Base.hpp"
#include "Mixture/Renderer/RendererInfo.hpp"

#include <glm/glm.hpp>

namespace Mixture
{
    // ================== Interface ==================
    class IShape
    {
    public:
        virtual ~IShape() = default;
        
        virtual void Tesselate(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) const = 0;
        virtual void HashCombine(std::size_t& seed) const = 0;
    };

    // ================== Rectangle ==================
    class Rectangle : public IShape
    {
    public:
        Rectangle(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color);
        
        void Tesselate(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) const override;
        void HashCombine(std::size_t& seed) const override;
        
    private:
        glm::vec3 m_Start, m_End;
        glm::vec4 m_Color;
    };

    // ================== Line ==================
    class Line : public IShape
    {
    public:
        Line(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, float thickness);
        
        void Tesselate(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) const override;
        void HashCombine(std::size_t& seed) const override;
        
    private:
        glm::vec3 m_Start, m_End;
        glm::vec4 m_Color;
        float m_Thickness;
    };
}
