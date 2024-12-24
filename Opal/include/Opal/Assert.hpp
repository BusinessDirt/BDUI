#pragma once

#include "Opal/Base.hpp"
#include "Opal/Log.hpp"

#include <filesystem>

#ifdef OPAL_ENABLE_ASSERTS

    // Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
    // provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
    #define OPAL_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { OPAL##type##ERROR(msg, __VA_ARGS__); OPAL_DEBUGBREAK(); } }
    #define OPAL_INTERNAL_ASSERT_WITH_MSG(type, check, ...) OPAL_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
    #define OPAL_INTERNAL_ASSERT_NO_MSG(type, check) OPAL_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", OPAL_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

    #define OPAL_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
    #define OPAL_INTERNAL_ASSERT_GET_MACRO(...) OPAL_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, OPAL_INTERNAL_ASSERT_WITH_MSG, OPAL_INTERNAL_ASSERT_NO_MSG)

    // Currently accepts at least the condition and one additional parameter (the message) being optional
    #define OPAL_ASSERT(...) OPAL_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__)
    #define OPAL_CORE_ASSERT(...) OPAL_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__)

#else

    #define OPAL_ASSERT(...)
    #define OPAL_CORE_ASSERT(...)

#endif
