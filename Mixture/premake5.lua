project "Mixture"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "off"

    pchheader "include/mxpch.hpp"
    pchsource "src/mxpch.cpp"

    files { "include/**.hpp", "src/**.cpp", "include/**.h", "src/**.mm" }

    includedirs { 
        "include"
    }

    externalincludedirs {
        "../Opal/include",

        "%{IncludeDir.spdlog}"
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

    filter "action:xcode4"
        links {
            "Cocoa.framework",          -- Cocoa framework (includes AppKit)
            "AppKit.framework",         -- Explicitly link AppKit for NSWindow
            "Foundation.framework",     -- Foundation framework for NSString
            "IOKit.framework",
            "QuartzCore.framework"
        }

        frameworkdirs { "/System/Library/Frameworks" }
        
        filter "files:**.mm"
            compileas "Objective-C++"  -- Compile .mm files as Objective-C++