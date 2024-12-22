#include "mxpch.hpp"
#include "Mixture/Core/Application.hpp"

namespace Mixture
{
    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& name, ApplicationCommandLineArgs args)
    {
        MX_CORE_ASSERT(!s_Instance, "Application already exisits!");
        s_Instance = this;
    }

    Application::~Application()
    {
    }

    void Application::Close()
    {
        m_Running = false;
    }

    void Application::Run()
    {
        
    }
}
