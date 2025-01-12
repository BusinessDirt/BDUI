#include "mxpch.hpp"
#include "Mixture/Renderer/LayerStack.hpp"

namespace Mixture
{
    LayerStack::~LayerStack()
    {
        for (Layer* layer : m_Layers)
        {
            layer->OnDetach();
            delete layer;
        }
    }

    void LayerStack::Update(FrameInfo& frameInfo) const
    {
        for (Layer* layer : m_Layers)
            layer->OnUpdate(frameInfo);
    }

    void LayerStack::OnEvent(Event& e) const
    {
        for (Layer* layer : m_Layers)
            layer->OnEvent(e);
    }


    void LayerStack::PushLayer(Layer* overlay)
    {
        m_Layers.emplace_back(overlay);
        overlay->OnAttach();
    }

    void LayerStack::PopLayer(Layer* overlay)
    {
        std::vector<Layer*>::iterator it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
        if (it != m_Layers.end())
        {
            overlay->OnDetach();
            m_Layers.erase(it);
        }
    }
}