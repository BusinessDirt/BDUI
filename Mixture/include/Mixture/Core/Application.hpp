#pragma once

int Entrypoint(int argc, char** argv);

namespace Mixture
{
    struct ApplicationCommandLineArgs
    {
        int count = 0;
        char** args = nullptr;

        const char* operator[](int index) const
        {
            MX_CORE_ASSERT(index < count);
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

            static Application& Get() { return *s_Instance; }

        private:
            void Run();

        private:
            bool m_Running = true;
    private:
        static Application* s_Instance;
        friend int ::Entrypoint(int argc, char** argv);
    };

    Application* CreateApplication(ApplicationCommandLineArgs args);
}
