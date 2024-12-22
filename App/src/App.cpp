#include "App.hpp"

#include <Mixture/Core/Entrypoint.hpp>

using namespace Mixture;

ExampleApp::ExampleApp(ApplicationCommandLineArgs args)
{
    
}

namespace Mixture
{
    Application* CreateApplication(ApplicationCommandLineArgs args)
    {
        ExampleApp* app = new ExampleApp(args);
        return app;
    }
}
