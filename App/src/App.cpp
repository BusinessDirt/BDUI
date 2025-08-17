#include <Mixture.hpp>
#include <Mixture/Core/Entrypoint.hpp>

#include "MainLayer.hpp"

namespace Mixture
{
    class ExampleApp : public Application
    {
    public:
        explicit ExampleApp(ApplicationCommandLineArgs args)
            : Application("App", args)
        {
            PushLayer(new MainLayer());
        }
    };

    Application* CreateApplication(ApplicationCommandLineArgs args)
    {
        ExampleApp* app = new ExampleApp(args);
        return app;
    }
}


