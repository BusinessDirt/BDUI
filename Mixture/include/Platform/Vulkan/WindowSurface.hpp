#pragma once

#include "Platform/Vulkan/Base.hpp"

namespace Mixture::Vulkan
{
    class WindowSurface
    {
    public:
        WindowSurface();
        ~WindowSurface();
        
        OPAL_NON_COPIABLE(WindowSurface);
        
    private:
        VULKAN_HANDLE(VkSurfaceKHR, m_Surface);
    };
}
