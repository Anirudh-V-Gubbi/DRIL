workspace "OpenGL"
  configurations {"debug", "release"}
  language "C++"
  cppdialect "C++20"
  targetdir "bin/%{cfg.buildcfg}"
  location "build"
  outputdir= "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

 defines {
  "RESOURCE_PATH=\"" .. _WORKING_DIR .. "/resources/" .. "\"",
  "DLL_PATH=\"" ..  _WORKING_DIR .. "/bin/%{cfg.buildcfg}" .. "\"",
  "APPLICATION_PATH=\"" ..  _WORKING_DIR .. "/application/" .. "\"",
  "OUTPUT_PATH=\"" .. _WORKING_DIR .. "/build/" .. "\"" 
  }

  -- Activates verbose mode
  if _OPTIONS["verbose"] then
    defines { "EFSW_VERBOSE" }
  end

  objdir("obj/" .. os.target() .. "/")

  include "../../lib"
  include "dep/GLFW"
  include "dep/Glad"

  filter "configurations:debug"
    defines { "DEBUG" }
    symbols "On"

  filter "configurations:release"
    defines { "RELEASE" }
    optimize "On"

  project "OpenGLApplication"
    kind "SharedLib"
    location "build"
    files { "application/*.cpp", "dep/stb_image/*.cpp"}
    includedirs { "dep/Glad/include/", "dep/glfw/include/", "src/", "dep/stb_image", "dep/glm"}
    links { 
      "GLFW",
      "Glad"
    }

    filter "system:windows"
      links {
        "opengl32",
        "shell32", 
        "gdi32"
      }
      defines {
        "DRIL_EXPORT=__declspec(dllexport)"
      }
    
    filter "system:linux"
      defines {
        "DRIL_EXPORT= "
      }
    
    filter "system:macosx"
      defines {
        "DRIL_EXPORT= "
      }
  
  project "OpenGLRuntime"
    kind "ConsoleApp"
    location "build"
    files { "src/*.cpp", "../../lib/*.cpp", "dep/stb_image/*.cpp"}
    includedirs {
      "dep/glm", 
      "dep/Glad/include/",
      "dep/glfw/include/",
      "dep/stb_image",
      "src/", "application/",
      "../../lib/",
      "../../dep/include",
      "../../dep/src"}
  
    links { 
      "GLFW",
      "Glad",
      "efsw-lib"
    }

    filter "configurations:debug"
      defines { "ENABLE_LOGGER" }

    filter "system:windows"
      links {
        "opengl32",
        "shell32", 
        "gdi32"
      }
      defines {
        "DRIL_EXPORT=__declspec(dllexport)"
      }
    
    filter "system:linux"
      defines {
        "DRIL_EXPORT= "
      }
    
    filter "system:macosx"
      defines {
        "DRIL_EXPORT= "
      }