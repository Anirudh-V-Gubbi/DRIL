workspace "OpenGL"
  configurations {"debug", "release"}
  language "C++"
  cppdialect "C++20"
  targetdir "bin/%{cfg.buildcfg}"
  location "build"
  outputdir= "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

 defines {
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

  filter "configurations:debug"
    defines { "DEBUG" }
    symbols "On"

  filter "configurations:release"
    defines { "RELEASE" }
    optimize "On"

  project "FileParserApplication"
    kind "SharedLib"
    location "build"
    files { "application/*.cpp", "dep/**.cpp" }
    includedirs { "dep/" }

  project "FileParserRuntime"
    kind "ConsoleApp"
    location "build"
    files { "src/*.cpp", "../../lib/*.cpp" }
    includedirs {
      "src/", "application/",
      "../../lib/",
      "../../dep/include",
      "../../dep/src"}
  
    links {
      "efsw-lib"
    }

    filter "configurations:debug"
      defines { "ENABLE_LOGGER" }