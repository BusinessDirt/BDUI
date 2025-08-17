#pragma once

#include "Mixture/Renderer/RendererInfo.hpp"

#include <glm/glm.hpp>

namespace Mixture
{
    // ================== Interface ==================
    class IShape
    {
    public:
        OPAL_NON_COPIABLE(IShape);
        
        IShape() = default;
        virtual ~IShape() = default;
        
        virtual void Tessellate(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) const = 0;
        virtual void HashCombine(std::size_t& seed) const = 0;
    };

    // ================== Rectangle ==================
    class Rectangle final : public IShape
    {
    public:
        Rectangle(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color);
        
        void Tessellate(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) const override;
        void HashCombine(std::size_t& seed) const override;
        
    private:
        glm::vec3 m_Start, m_End;
        glm::vec4 m_Color;
    };

    // ================== Line ==================
    class Line final : public IShape
    {
    public:
        Line(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, float thickness);
        
        void Tessellate(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) const override;
        void HashCombine(std::size_t& seed) const override;
        
    private:
        glm::vec3 m_Start, m_End;
        glm::vec4 m_Color;
        float m_Thickness;
    };
}
