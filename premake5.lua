require "external/premake-ecc/ecc"

workspace "GNL_Workspace"
    configurations { "Debug", "Release" }
    platforms { "Dev", "42" }
    toolset "clang"

    -- concat path with premake action run (gmake)
    location ("build/" .. _ACTION)

    buildoptions {"-Wall", "-Werror", "-Wextra"}

    filter "platforms:Dev"
        architecture "ARM64"
        system "linux"

    filter "platforms:42"
        architecture "x86_64"
        system "linux"
        buildoptions { "--target=x86_64-linux-gnu" }
        linkoptions { "--target=x86_64-linux-gnu", "-fuse-ld=lld" }

    filter {}

project "get_next_line"
    kind "StaticLib"
    language "C"
    targetdir "bin/%{cfg.platform}/%{cfg.buildcfg}"
    objdir "build/%{cfg.platform}/%{cfg.buildcfg}"

    -- Update paths to the new subdirectory
    files {
        "src/turnin/get_next_line.c",
        "src/turnin/get_next_line_utils.c",
        "src/turnin/get_next_line.h"
    }

    -- so the library itself can find its own header
    includedirs { "src/turnin" }

project "test_runner"
    kind "ConsoleApp"
    language "C"
    targetdir "bin/%{cfg.platform}/%{cfg.buildcfg}"
    objdir "build/%{cfg.platform}/%{cfg.buildcfg}"

    files { "tests/own/main.c" }

    -- allows main.c to use #include "get_next_line.h"
    includedirs { "src/turnin" }
    links { "get_next_line" }
