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

        "%{IncludeDir.spdlog}",
        "%{IncludeDir.Vulkan}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}"
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
        
        filter "files:**.mm"
            compileas "Objective-C++"  -- Compile .mm files as Objective-C++