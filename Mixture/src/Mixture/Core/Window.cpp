#include "mxpch.hpp"
#include "Mixture/Core/Window.hpp"

#include "Mixture/Core/Base.hpp"

#if defined(OPAL_PLATFORM_WINDOWS)
    #include "Platform/Windows/WindowsWindow.hpp"
#elif defined(OPAL_PLATFORM_LINUX)
    #include "Platform/Linux/LinuxWindow.hpp"
#elif defined(OPAL_PLATFORM_DARWIN)
    #include "Platform/Darwin/DarwinWindow.h"
#endif

namespace Mixture
{
    Scope<Window> Window::Create(const WindowProps& props)
    {
    #if defined(OPAL_PLATFORM_WINDOWS)
        return CreateScope<WindowsWindow>(props);
    #elif defined(OPAL_PLATFORM_LINUX)
        return CreateScope<LinuxWindow>(props);
    #elif defined(OPAL_PLATFORM_DARWIN)
        return CreateScope<DarwinWindow>(props);
    #else
        OPAL_CORE_ERROR("Could not create window: Platform not supported!");
    #endif
    }

    void Window::SetEventCallback(const std::function<void (Event &)> &callback)
    {
        m_EventCallback = callback;
    }

} // namespace Onyx
