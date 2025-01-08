project "Opal"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "off"

    files { "include/**.hpp", "src/**.cpp" }

    includedirs { 
        "include"
    }

    externalincludedirs {
        "%{IncludeDir.spdlog}"
    }

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    filter "configurations:Debug"
        defines { "OPAL_DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "OPAL_RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "On"

    filter "configurations:Dist"
        defines { "OPAL_DIST" }
        runtime "Release"
        optimize "On"
        symbols "Off"

    -- Add the /utf-8 flag
    filter "action:vs2022" -- Only apply for MSVC toolset
        buildoptions { "/utf-8" }