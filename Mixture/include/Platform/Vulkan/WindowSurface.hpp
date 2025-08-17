#pragma once

#include "Platform/Vulkan/Base.hpp"

namespace Mixture::Vulkan
{
    class WindowSurface
    {
    public:
        OPAL_NON_COPIABLE(WindowSurface);
        
        WindowSurface();
        ~WindowSurface();
        
    private:
        VULKAN_HANDLE(VkSurfaceKHR, m_Surface);
    };
}
