#include "mxpch.hpp"
#include "Mixture/Core/Application.hpp"

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
        
        m_Window = Window::Create(props);
        m_Window->SetEventCallback(OPAL_BIND_EVENT_FN(OnEvent));

        m_AssetManager = CreateScope<AssetManager>();
        
        m_VulkanContext = CreateScope<Vulkan::Context>();
        m_VulkanContext->Initialize(name);
    }

    Application::~Application()
    {
        m_VulkanContext.reset();
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
        }
    }

    void Application::OnEvent(Event& event)
    {
        // Dispatch events to specific handlers
        EventDispatcher dispatcher(event);

        // Handle window close and resize event
        dispatcher.Dispatch<WindowCloseEvent>(OPAL_BIND_EVENT_FN(OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(OPAL_BIND_EVENT_FN(OnWindowResize));
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        if (e.IsFinished()) OPAL_CORE_INFO("{}", e.ToString());
        return m_VulkanContext->OnWindowResize(e);
    }
}
