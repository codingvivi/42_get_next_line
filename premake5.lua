workspace "GNL_Workspace"
    configurations { "Debug", "Release" }
    platforms { "Dev", "42" }
    toolset "clang"
    buildoptions {"-Wall","-Werror","-Wextra", "-l"}
    linkoptions {"-Wall","-Werror","-Wextra", "-l"}
    -- Workspace-level filters apply to all projects
    filter "platforms:Dev"
        architecture "ARM64"
        system "linux"

    filter "platforms:42"
        architecture "x86_64"
        system "linux"
        buildoptions { "-l", "--target=x86_64-linux-gnu" }
        linkoptions { "--target=x86_64-linux-gnu", "-fuse-ld=lld" }

    -- Reset filter so project declarations aren't trapped
    --filter {}

project "get_next_line"
    kind "StaticLib"
    language "C"
    targetdir "bin/%{cfg.platform}/%{cfg.buildcfg}"
    objdir "build/%{cfg.platform}/%{cfg.buildcfg}"
    buildoptions {"-Wall","-Werror","-Wextra", "-l"}
    files {
        "src/get_next_line.c",
        "src/get_next_line_utils.c",
        "src/get_next_line.h"
    }

project "test_runner"
    kind "ConsoleApp"
    language "C"
    targetdir "bin/%{cfg.platform}/%{cfg.buildcfg}"
    objdir "build/%{cfg.platform}/%{cfg.buildcfg}"
    files { "tests/own/main.c" }
    includedirs { "src" }
    links { "get_next_line" }
