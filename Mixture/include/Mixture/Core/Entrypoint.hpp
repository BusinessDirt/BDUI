#pragma once

#include "Mixture/Core/Base.hpp"
#include "Mixture/Core/Application.hpp"

extern Mixture::Application* Mixture::CreateApplication(Mixture::ApplicationCommandLineArgs args);

int Entrypoint(int argc, char** argv)
{
    Opal::Log::Init();
    
    Mixture::Application* app = Mixture::CreateApplication({ argc, argv });
    app->Run();
    delete app;

    return 0;
}

#ifdef OPAL_DIST

#if defined(OPAL_PLATFORM_WINDOWS)

#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    return Entrypoint(__argc, __argv);
}

#elif defined(OPAL_PLATFORM_MACOSX)



#else
#error OS not supported
#endif // MX_PLATFORM_WINDOWS

#else

int main(int argc, char** argv)
{
    return Entrypoint(argc, argv);
}

#endif // MX_DIST
