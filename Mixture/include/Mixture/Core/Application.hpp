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
        int Count = 0;
        char** Args = nullptr;

        const char* operator[](const int index) const
        {
            OPAL_CORE_ASSERT(index < Count)
            return Args[index];
        }
    };

    class Application
    {
    public:
        OPAL_NON_COPIABLE(Application);
        
        explicit Application(const std::string& name = "Mixture App", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
        virtual ~Application();

        void Close();
        
        void OnEvent(Event& event);

        static Application& Get() { return *s_Instance; }
        OPAL_NODISCARD const Window& GetWindow() const { return *m_Window; }
        OPAL_NODISCARD const AssetManager& GetAssetManager() const { return *m_AssetManager; }
        
        void PushLayer(Layer* layer) const { m_LayerStack->PushLayer(layer); }
        void PopLayer(Layer* layer) const { m_LayerStack->PopLayer(layer); }

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
