workspace "GNL_Workspace"
    configurations { "Debug", "Release" }
    platforms { "Dev", "42" }
    toolset "clang"


    buildoptions {"-Wall", "-Werror", "-Wextra", "-fPIE" }

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

    files { "tests/own/main.c" }

    -- allows main.c to use #include "get_next_line.h"
    includedirs { "src/turnin" }
    links { "get_next_line" }
