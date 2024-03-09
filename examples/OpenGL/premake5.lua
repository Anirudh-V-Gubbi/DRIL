workspace "OpenGL"
  configurations {"Debug", "Release"}
  language "C++"
  targetdir "bin/%{cfg.buildcfg}"
  location "build"
  outputdir= "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
  include "dep/GLFW"
  include "dep/Glad"
  links {"OpenGLlib"}

  filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

  filter "configurations:Release"
    defines { "RELEASE" }
    optimize "On"
  
  project "OpenGLlib"
    kind "SharedLib"
    location "build"
    files
    {
      "dep/stb_image/stb_image.cpp",
      "dep/stb_image/stb_image.h",
      "dep/glm/**.h",
      "dep/glm/**.inl"
    }
    includedirs
    {
      "dep/GLFW",
      "dep/glad",
      "dep/glm",
      "dep/stb_image"
    }
    

  project "OpenGLRuntime"
    kind "ConsoleApp"
    location "build"
    files { "main.cpp"}
    includedirs { "dep/Glad/include/"}

    filter "system:windows"
      links { 
        "OpenGLlib","GLFW","opengl32",
        "shell32", 
        "gdi32"
      }
    
    filter "system:macosx"
      links { 
        "GLFW",
        "Glad"
      }
