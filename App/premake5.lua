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

    externalincludedirs {
        "../Opal/include",
        "../Mixture/include",

        "%{IncludeDir.spdlog}"
    }

    links {
        "Opal",
        "Mixture"
    }

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "On"

    filter "configurations:Dist"
        defines { "DIST" }
        runtime "Release"
        optimize "On"
        symbols "Off"

    -- mac specific settings
    filter "action:xcode4"
    links {
        "Cocoa.framework",
        "Foundation.framework",
        "IOKit.framework",
        "QuartzCore.framework",
        "AppKit.framework"
    }

    frameworkdirs { "/System/Library/Frameworks" }

    xcodebuildsettings {
        ["LD_RUNPATH_SEARCH_PATHS"] = "@executable_path/../Frameworks @loader_path/../Frameworks"
    }

    filter "files:**.mm"
        compileas "Objective-C++"