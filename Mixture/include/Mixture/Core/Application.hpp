#pragma once

#include "Mixture/Assets/AssetManager.hpp"
#include "Mixture/Core/Window.hpp"

#include "Mixture/Events/Event.hpp"
#include "Mixture/Events/ApplicationEvent.hpp"

#include "Mixture/Renderer/LayerStack.hpp"
#include "Mixture/Renderer/RendererInfo.hpp"

int Entrypoint(int argc, char** argv);

namespace Mixture
{
    struct ApplicationCommandLineArgs
    {
        int count = 0;
        char** args = nullptr;

        const char* operator[](int index) const
        {
            OPAL_CORE_ASSERT(index < count);
            return args[index];
        }
    };

    class Application
    {
    public:
        Application(const std::string& name = "Mixture App", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
        virtual ~Application();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        void Close();
        
        void OnEvent(Event& event);

        static Application& Get() { return *s_Instance; }
        const Window& GetWindow() const { return *m_Window; }
        const AssetManager& GetAssetManager() const { return *m_AssetManager; }
        
        void PushLayer(Layer* layer) { m_LayerStack->PushLayer(layer); };
        void PopLayer(Layer* layer) { m_LayerStack->PopLayer(layer); }

    private:
        void Run() const;
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnFramebufferResize(const FramebufferResizeEvent& e);

    private:
        Scope<Window> m_Window;
        Scope<AssetManager> m_AssetManager;
        Scope<LayerStack> m_LayerStack;
        
        bool m_Running = true;
    private:
        static Application* s_Instance;
        friend int ::Entrypoint(int argc, char** argv);
    };

    Application* CreateApplication(ApplicationCommandLineArgs args);
}
