#include "mxpch.hpp"
#include "Mixture/Core/Application.hpp"

#include "Mixture/Renderer/Renderer.hpp"

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
    }

    Application::~Application()
    {
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
        while (m_Running)
        {
            m_Window->OnUpdate();

            Renderer::DrawFrame();
        }
    }

    void Application::OnEvent(Event& event)
    {
        // Dispatch events to specific handlers
        EventDispatcher dispatcher(event);

        // Handle window close and resize event
        dispatcher.Dispatch<WindowCloseEvent>(OPAL_BIND_EVENT_FN(OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(OPAL_BIND_EVENT_FN(OnWindowResize));

        Renderer::OnEvent(event);
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
        return false;
    }
}
