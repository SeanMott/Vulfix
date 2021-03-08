workspace "Vulfix"
    --architecture "x86_64"
    architecture "x86"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includeDir = {}
includeDir["GLFW"] = "Library/GLFW/Include"
--includeDir["Glad"] = "Library/Glad/include"
--includeDir["stb"] = "Library/stb"
includeDir["glm"] = "Library/glm"
includeDir["vulkan"] = "Library/vulkan/Include"
--includeDir["FastDelegate"] = "Library/FastDelegate"
--includeDir["entt"] = "Library/entt/src"
--includeDir["meta"] = "Library/meta/src"
--includeDir["ImGUI"] = "Library/imgui"

--includes the premake files
include "Library/GLFW"
--include "Library/Glad"
--include "Library/stb"
--include "Library/ImGui"

--the lib for Vulfix
project "Vulfix"
    location "Vulfix"
    kind "StaticLib"
    language "C"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

    --pchheader "smpch.h"
    --pchsource "%{prj.name}/src/smpch.cpp"

    files 
    {
        "%{prj.name}/includes/**.h",
        --"%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.c",
        --"%{prj.name}/src/**.cpp"
    }
    
    includedirs
    {
        --"%{includeDir.Glad}",
        "%{includeDir.GLFW}",
        "%{includeDir.vulkan}",
        "%{includeDir.glm}",
        --"%{includeDir.stb}",
        --"%{includeDir.FastDelegate}",
        --"%{includeDir.entt}",
        --"%{includeDir.meta}",
        "Vulfix/includes"
    }
    
    links
    {
        --"StbImage",
        --"Glad",
        "GLFW",
        --"opengl32.lib"
        "../../VulkanSDK/1.2.148.1/lib32/vulkan-1.lib"
    }

    defines
    {
        "GLM_FORCE_RADIANS",
        "GLM_FORCE_DEPTH_ZERO_TO_ONE"
    }

    filter "system:windows"
        --cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
    
        defines
        {
            "Window_Build",
            "Enable_Vulkan",
            "GLFW_INCLUDE_VULKAN",
        }
    
    filter "configurations:Debug"
        defines "VULFIX_DEBUG"
        symbols "On"
    
    filter "configurations:Release"
        defines "VULFIX_RELEASE"
        optimize "On"
    
    filter "configurations:Dist"
        defines "VULFIX_DIST"
        optimize "On"

--Sandbox used to test Vulfix
project "Sandbox"
location "Sandbox"
kind "ConsoleApp"
language "C++"

targetdir ("bin/" .. outputdir .. "/%{prj.name}")
objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

--pchheader "smpch.h"
--pchsource "%{prj.name}/src/smpch.cpp"

files 
{
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.hpp",
    "%{prj.name}/src/**.c",
    "%{prj.name}/src/**.cpp"
}

includedirs
{
    "%{includeDir.glm}",
    "%{includeDir.GLFW}",
    "%{includeDir.vulkan}",
    --"%{includeDir.FastDelegate}",
    --"%{includeDir.entt}",
    --"%{includeDir.ImGUI}",
    "Vulfix/includes",
    "Sandbox/src"
}

links
{
    "Vulfix",
}

defines
{
    "GLM_FORCE_RADIANS",
    "GLM_FORCE_DEPTH_ZERO_TO_ONE"
}

filter "system:windows"
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"

    defines
    {
        "Window_Build",
        "Enable_Vulkan",
        "GLFW_INCLUDE_VULKAN",
    }

filter "configurations:Debug"
    defines "VULFIX_DEBUG"
    symbols "On"

filter "configurations:Release"
    defines "VULFIX_RELEASE"
    optimize "On"

filter "configurations:Dist"
    defines "VULFIX_DIST"
    optimize "On"
