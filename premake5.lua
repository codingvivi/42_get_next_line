require "external/build/premake-ecc/ecc"

workspace "GNL_Workspace"
    configurations {"Release", "Debug1b", "Debug10b", "Debug1mb" }
    platforms { "Dev", "42" }
    toolset "clang"

    --defines {  "BUFFER_SIZE=1" }

    buildoptions {"-Wall", "-Werror", "-Wextra"}
    linkoptions {"-fPIE"}

    filter "platforms:Dev"
        architecture "ARM64"
        system "linux"

    filter "platforms:42"
        architecture "x86_64"
        system "linux"
        buildoptions { "--target=x86_64-linux-gnu" }
        linkoptions { "--target=x86_64-linux-gnu", "-fuse-ld=lld" }

    filter "configurations:Debug1b"
        buildoptions {"-g"}
        defines {"BUFFER_SIZE=1"}

    filter "configurations:Debug10b"
        buildoptions {"-g"}
        defines {"BUFFER_SIZE=10"}

    filter "configurations:Debug1mb"
        buildoptions {"-g"}
        defines {"BUFFER_SIZE=1000000"}



project "get_next_line"
    kind "StaticLib"
    language "C"

    -- Update paths to the new subdirectory
    files {
        "src/get_next_line.c",
        "src/get_next_line_utils.c",
        "src/get_next_line.h"
    }

    -- so the library itself can find its own header
    includedirs { "src/turnin" }

project "test_runner"
    kind "ConsoleApp"
    language "C"

    files { "tests/main.c" }

    -- allows main.c to use #include "get_next_line.h"
    includedirs { "src" }
    links { "get_next_line" }


