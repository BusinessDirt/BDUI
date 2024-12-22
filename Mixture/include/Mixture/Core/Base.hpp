#pragma once

#include "Mixture/Core/PlatformDetection.hpp"

#include <memory>
#include <vector>

// DEBUG
#ifdef MX_DEBUG
    #if defined(MX_PLATFORM_WINDOWS)
        #define MX_DEBUGBREAK() __debugbreak()
    #elif defined(MX_PLATFORM_LINUX)
        #include <signal.h>
        #define MX_DEBUGBREAK() raise(SIGTRAP)
    #elif defined(MX_PLATFORM_MACOSX)
        #define MX_DEBUGBREAK() __builtin_debugtrap()
    #else
        #error "Platform doesn't support debugbreak yet!"
    #endif
    #define MX_ENABLE_ASSERTS
#else
    #define MX_DEBUGBREAK()
#endif

#define MX_EXPAND_MACRO(x) x
#define MX_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)
#define MX_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#define MX_NON_COPIABLE(name) name(const name&) = delete; \
    name& operator=(const name&) = delete; \
    name(name&&) = delete; \
    name& operator=(name&&) = delete

#define VULKAN_HANDLE(VulkanHandleType, name) \
    public: VulkanHandleType GetHandle() const { return name; } \
    private: VulkanHandleType name = VK_NULL_HANDLE


namespace Mixture
{

    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Vector = std::vector<T>;
}

#include "Mixture/Core/Log.hpp"
#include "Mixture/Core/Assert.hpp"
