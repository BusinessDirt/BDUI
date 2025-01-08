project "App"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "off"

    files { "include/**.hpp", "src/**.cpp" }

    includedirs { 
        "include"
    }

    libdirs { "%{LibraryDir.Vulkan}" }

    externalincludedirs {
        "../Opal/include",
        "../Mixture/include",

        "%{IncludeDir.spdlog}",
        "%{IncludeDir.Vulkan}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}"
    }

    links {
        "Opal",
        "Mixture"
    }

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    filter "configurations:Debug"
        defines { "OPAL_DEBUG" }
        runtime "Debug"
        symbols "On"
        filter "system:windows"
            links {
                "%{Library.ShaderC_Debug}",
                "%{Library.SPIRV_Cross_Debug}"
            }

    filter "configurations:Release"
        defines { "OPAL_RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "On"
        filter "system:windows"
            links {
                "%{Library.ShaderC_Release}",
                "%{Library.SPIRV_Cross_Release}"
            }

    filter "configurations:Dist"
        defines { "OPAL_DIST" }
        runtime "Release"
        optimize "On"
        symbols "Off"
        filter "system:windows"
            links {
                "%{Library.ShaderC_Release}",
                "%{Library.SPIRV_Cross_Release}"
            }

    -- windows specific settings
    filter "system:windows"
        links { "%{Library.Vulkan}" }

    -- mac specific settings
    filter "action:xcode4"
        local vulkanFW = os.getenv("VULKAN_SDK") .. "/Frameworks"
        links {
            "vulkan.framework",
            "Cocoa.framework",
            "Foundation.framework",
            "IOKit.framework",
            "QuartzCore.framework",
            "AppKit.framework"
        }

        frameworkdirs { 
            vulkanFW,
            "/System/Library/Frameworks" 
        }

        xcodebuildsettings {
            ["LD_RUNPATH_SEARCH_PATHS"] = "@executable_path/../Frameworks @loader_path/../Frameworks " .. vulkanFW
        }

    -- Add the /utf-8 flag
    filter "action:vs2022" -- Only apply for MSVC toolset
        buildoptions { "/utf-8" }