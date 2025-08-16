#include "mxpch.hpp"
#include "Mixture/Core/Application.hpp"

#include "Mixture/Renderer/Renderer.hpp"
#include "Mixture/Core/Time.hpp"

#include <Opal/Base.hpp>

namespace Mixture
{
    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& name, ApplicationCommandLineArgs args)
    {
        OPAL_CORE_ASSERT(!s_Instance, "Application already exisits!");
        
        s_Instance = this;
        
        WindowProps props = WindowProps();
        props.Title = name;
        
        m_Window = CreateScope<Window>(props);
        m_Window->SetEventCallback(OPAL_BIND_EVENT_FN(OnEvent));

        m_AssetManager = CreateScope<AssetManager>();
        
        Renderer::Init(name);
        
        m_LayerStack = CreateScope<LayerStack>();
    }

    Application::~Application()
    {
        m_LayerStack.reset();
        
        Renderer::Shutdown();
        
        m_AssetManager.reset();
        m_Window.reset();
    }

    void Application::Close()
    {
        m_Running = false;
    }

    void Application::Run()
{
        Timer frameTimer{};
        while (m_Running)
        {
            // Update information about current frame
            m_FrameInfo.FrameTime = frameTimer.Tick();
            
            // Update everything
            m_Window->OnUpdate();
            m_LayerStack->Update(m_FrameInfo);

            // Render after updating
            Renderer::DrawFrame(m_FrameInfo, *m_LayerStack);
            
        }
    }

    void Application::OnEvent(Event& event)
    {
        // Dispatch events to specific handlers
        EventDispatcher dispatcher(event);

        // Handle window close and resize event
        dispatcher.Dispatch<WindowCloseEvent>(OPAL_BIND_EVENT_FN(OnWindowClose));
        dispatcher.Dispatch<FramebufferResizeEvent>(OPAL_BIND_EVENT_FN(OnFramebufferResize));

        m_LayerStack->OnEvent(event);
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnFramebufferResize(FramebufferResizeEvent& e)
    {
        Renderer::OnFramebufferResize(e.GetWidth(), e.GetHeight());
        return false;
    }
}
