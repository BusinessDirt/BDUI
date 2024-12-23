#include "mxpch.hpp"
#include "Mixture/Core/Application.hpp"

#include <Opal/Base.hpp>

namespace Mixture
{
    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& name, ApplicationCommandLineArgs args)
    {
        OPAL_CORE_ASSERT(!s_Instance, "Application already exisits!");
        
        WindowProps props = WindowProps();
        props.Title = name;
        m_Window = Window::Create(props);
        m_Window->SetEventCallback([this](Event& event) { OnEvent(event); });
        
        s_Instance = this;
    }

    Application::~Application()
    {
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
        }
    }

    void Application::OnEvent(Event& event)
    {
        // Dispatch events to specific handlers
        EventDispatcher dispatcher(event);

        // Handle window close event
        dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) { return OnWindowClose(e); });
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }
}
