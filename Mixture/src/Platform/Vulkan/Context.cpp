#include "mxpch.hpp"
#include "Platform/Vulkan/Context.hpp"

#include <Opal/Base.hpp>

namespace Vulkan
{
    namespace Util
    {
        static std::vector<const char*> GetRequiredLayers()
        {
            std::vector<const char*> requiredLayers;

            // Common layers for validation
        #ifndef OPAL_DIST
            requiredLayers.push_back("VK_LAYER_KHRONOS_validation");
        #endif

            return requiredLayers;
        }

        static std::vector<const char*> GetRequiredExtensions()
        {
            std::vector<const char*> requiredExtensions;

            // Common extensions
            requiredExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
            
        #if defined(OPAL_PLATFORM_WINDOWS)
            requiredExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
        #elif defined(OPAL_PLATFORM_DARWIN)
            requiredExtensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
            requiredExtensions.push_back("VK_EXT_metal_surface");
        #elif defined(OPAL_PLATFORM_LINUX)
            requiredExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
            requiredExtensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
            requiredExtensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
        #endif

        #ifndef OPAL_DIST
            requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        #endif

            return requiredExtensions;
        }
    }

    Context::~Context()
    {
        m_DebugMessenger = nullptr;
        m_Instance = nullptr;
    }

    void Context::Initialize(const std::string& applicationName)
    {
        // Create Instance
        std::vector<const char*> requiredLayers = Util::GetRequiredLayers();
        std::vector<const char*> requiredExtensions = Util::GetRequiredExtensions();
        m_Instance.reset(new Instance(applicationName, requiredLayers, requiredExtensions));
        
    #ifndef OPAL_DIST
        m_DebugMessenger.reset(new DebugMessenger(m_Instance->GetHandle()));
    #endif
    }
}
