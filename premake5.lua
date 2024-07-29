-- premake5.lua
workspace "SpaceWarGame"
	architecture "x64"
	startproject "SpaceWarGame"

	configurations
	{
		"Debug",
		"Release",
		"RelWithDebInfo",
	}
	
	flags
	{
		"MultiProcessorCompile",
	}

   -- Workspace-wide build options for MSVC
   filter "system:windows"
		buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }
	  
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/glad"
group ""

-- Dependency directories
DependencyDir = {}
DependencyDir["SDL2"] = "vendor/SDL2"
DependencyDir["SDL2Image"] = "vendor/SDL2Image"
DependencyDir["SDL2_ttf"] = "vendor/SDL2_ttf"
DependencyDir["SDL2_mixer"] = "vendor/SDL2_mixer"
DependencyDir["entt"] = "vendor/entt"
DependencyDir["glm"] = "vendor/glm"
DependencyDir["rapidyaml"] = "vendor/rapidyaml"
DependencyDir["spdlog"] = "vendor/spdlog"
DependencyDir["glad"] = "vendor/glad"
DependencyDir["stb"] = "vendor/stb"

-- Project
project "SpaceWarGame"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++17"
    targetdir "bin/%{cfg.buildcfg}"
    staticruntime "on"
	ignoredefaultlibraries { "MSVCRT", "MSVCRTD" }

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{ 
		"src/**.h", 
		"src/**.cpp",
		"%{DependencyDir.rapidyaml}/include/**.hpp",
		"%{DependencyDir.entt}/include/**.hpp",
		"%{DependencyDir.glm}/**.hpp",
		"%{DependencyDir.glm}/**.inl",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"_NO_CRT_STDIO_INLINE",
	}

	includedirs
	{
		"src/",
		"src/engine/",
		"src/game/",
	}

	externalincludedirs
	{
		"%{DependencyDir.SDL2}/include",
		"%{DependencyDir.SDL2Image}/include",
		"%{DependencyDir.SDL2_ttf}/include",
		"%{DependencyDir.SDL2_mixer}/include",
		"%{DependencyDir.entt}/include",
		"%{DependencyDir.rapidyaml}/include",
		"%{DependencyDir.glm}/include",
		"%{DependencyDir.spdlog}/include",
		"%{DependencyDir.glad}/include",
		"%{DependencyDir.stb}/include",
	}

	libdirs
	{
		"%{DependencyDir.SDL2}/lib/x64",
		"%{DependencyDir.SDL2Image}/lib/x64",
		"%{DependencyDir.SDL2_ttf}/lib/x64",
		"%{DependencyDir.SDL2_mixer}/lib/x64",
	}

	links
	{
		"SDL2",
		"SDL2Main",
		"SDL2_image",
		"SDL2_ttf",
		"SDL2_mixer",
		"Glad",
		"opengl32.lib",
	}

	flags { "NoPCH" }
	
	filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }
	   postbuildcommands
	   {
		   "{COPY} %{DependencyDir.SDL2}/lib/x64/*.dll bin/" .. outputdir .. "/%{prj.name}",
		   "{COPY} %{DependencyDir.SDL2Image}/lib/x64/*.dll bin/" .. outputdir .. "/%{prj.name}",
		   "{COPY} %{DependencyDir.SDL2_ttf}/lib/x64/*.dll bin/" .. outputdir .. "/%{prj.name}",
		   "{COPY} %{DependencyDir.SDL2_mixer}/lib/x64/*.dll bin/" .. outputdir .. "/%{prj.name}",
		   "{COPY} res bin/" .. outputdir .. "/%{prj.name}/res",
	   }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

	filter "configurations:RelWithDebInfo"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "Off"

