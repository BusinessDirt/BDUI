#include "mxpch.hpp"
#include "Platform/Vulkan/Context.hpp"

#include <Opal/Base.hpp>

#ifdef OPAL_PLATFORM_DARWIN
    #include <vulkan/vulkan_metal.h>
#endif

#ifdef OPAL_PLATFORM_WINDOWS
    #include <vulkan/vulkan_win32.h>
#endif

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

        static std::vector<const char*> GetRequiredInstanceExtensions()
        {
            std::vector<const char*> requiredExtensions;

            // Common extensions
            requiredExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
            
        #if defined(OPAL_PLATFORM_WINDOWS)
            requiredExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
        #elif defined(OPAL_PLATFORM_DARWIN)
            requiredExtensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
            requiredExtensions.push_back(VK_EXT_METAL_SURFACE_EXTENSION_NAME);
        #elif defined(OPAL_PLATFORM_LINUX)
            requiredExtensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
        #endif

        #ifndef OPAL_DIST
            requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        #endif

            return requiredExtensions;
        }
    
        static std::vector<const char*> GetRequiredDeviceExtensions()
        {
            std::vector<const char*> requiredExtensions;
            
            // Swapchain
            requiredExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
            
        #ifdef OPAL_PLATFORM_DARWIN
            // Required for MoltenVK
            requiredExtensions.push_back("VK_KHR_portability_subset");
        #endif

            return requiredExtensions;
        }
    }

    Context::~Context()
    {
        m_GraphicsPipeline.reset();
        m_Swapchain.reset();
        m_Device.reset();
        m_PhysicalDevice.reset();
        m_WindowSurface.reset();
        m_DebugMessenger.reset();
        m_Instance.reset();
    }

    void Context::Initialize(const std::string& applicationName)
    {
        m_Instance.reset(new Instance(applicationName, Util::GetRequiredLayers(), Util::GetRequiredInstanceExtensions()));
        
    #ifndef OPAL_DIST
        m_DebugMessenger.reset(new DebugMessenger(m_Instance->GetHandle()));
    #endif
        
        m_WindowSurface.reset(new WindowSurface(m_Instance->GetHandle()));
        m_PhysicalDevice.reset(new PhysicalDevice(m_Instance->GetHandle(), m_WindowSurface->GetHandle(), Util::GetRequiredDeviceExtensions()));
        m_Device.reset(new Device(*m_PhysicalDevice, Util::GetRequiredLayers(), Util::GetRequiredDeviceExtensions()));
        m_Swapchain.reset(new Swapchain(*m_PhysicalDevice, m_Device->GetHandle(), m_WindowSurface->GetHandle()));
        m_GraphicsPipeline.reset(new GraphicsPipeline());
    }

    bool Context::OnWindowResize(Mixture::WindowResizeEvent& event)
    {
        return false;
    }
}
