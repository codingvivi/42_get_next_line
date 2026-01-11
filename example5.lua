workspace "GNL_Workspace"
    configurations { "Debug", "Release" }
    platforms { "ARM64", "x64" }
    toolset "clang"

-- 1. THE LIBRARY (The files you turn in)
project "get_next_line"
    kind "StaticLib"
    language "C"
    targetdir "bin/%{cfg.platform}/%{cfg.buildcfg}"
    files { "src/*.c", "src/*.h" }
    includedirs { "src" }

    filter "platforms:x64"
        architecture "x86_64"
        buildoptions { "--target=x86_64-linux-gnu" }

-- 2. THE TESTER (The executable you run)
project "test_runner"
    kind "ConsoleApp"
    language "C"
    targetdir "bin/%{cfg.platform}/%{cfg.buildcfg}"
    -- Point this to wherever your main.c or tester file is
    files { "tests/main.c" }
    includedirs { "src" }     -- So the tester can find get_next_line.h
    links { "get_next_line" } -- Automatically links your functions

    filter "platforms:x64"
        architecture "x86_64"
        buildoptions { "--target=x86_64-linux-gnu" }
        linkoptions  { "--target=x86_64-linux-gnu", "-fuse-ld=lld" }
