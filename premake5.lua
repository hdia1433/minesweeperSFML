newoption {
    trigger = "config",
    value = "CONFIG",
    description = "Build configuration",
    allowed = {
        { "debug",   "Debug build" },
        { "release", "Release build" },
    },
    default = "debug",
}

local cfg = _OPTIONS["config"] or "debug"

workspace("minesweeper")
architecture("ARM64")
if cfg == "release" then
configurations({"Release",  "Debug"})
else
configurations({ "Debug", "Release" })
end
startproject("minesweeper")

project("minesweeper")
kind("WindowedApp")
language("C++")
cppdialect("C++23")
targetdir("bin/%{cfg.buildcfg}")
objdir("bin-int/%{cfg.buildcfg}")

buildoptions({ "-std=c++23 ", "-MMD", "-MP" })

prebuildcommands({
	"mkdir -p bin-int/Debug && cp src/headers/pch.hpp bin-int/Debug/pch.hpp",
	"mkdir -p bin-int/Release && cp src/headers/pch.hpp bin-int/Release/pch.hpp",
})

if cfg == "release" then
postbuildcommands({
    "mkdir -p bin/%{cfg.buildcfg}/minesweeper.app/Contents/MacOS",
    "mkdir -p bin/%{cfg.buildcfg}/minesweeper.app/Contents/Resources",
    "cp bin/%{cfg.buildcfg}/minesweeper bin/%{cfg.buildcfg}/minesweeper.app/Contents/MacOS/minesweeper",
    "cp Info.plist bin/%{cfg.buildcfg}/minesweeper.app/Contents/Info.plist",
    "mkdir -p bin/%{cfg.buildcfg}/minesweeper.app/Contents/Frameworks",
    "cp /opt/homebrew/Cellar/sfml/3.0.1/lib/libsfml-graphics.3.0.1.dylib bin/%{cfg.buildcfg}/minesweeper.app/Contents/Frameworks/",
    "cp /opt/homebrew/Cellar/sfml/3.0.1/lib/libsfml-window.3.0.1.dylib bin/%{cfg.buildcfg}/minesweeper.app/Contents/Frameworks/",
    "cp /opt/homebrew/Cellar/sfml/3.0.1/lib/libsfml-system.3.0.1.dylib bin/%{cfg.buildcfg}/minesweeper.app/Contents/Frameworks/",
})
else
postbuildcommands({
            "ninja -t compdb > compile_commands.json",
    })
end
-- Source files
files({
	"src/**.cpp",
	"src/headers/**.hpp",
	"externals/**.cpp",
	"externals/**.h",
})

includedirs({
	"src/headers",
	"/opt/homebrew/include",
	"/opt/homebrew/include/SFML",
	"externals/imgui-sfml",
	"externals/imgui",
})

links({
	"sfml-graphics",
	"sfml-window",
	"sfml-system",
	"OpenGL.framework",
	"Cocoa.framework",
	"CoreFoundation.framework",
	"CoreGraphics.framework",
})
pchheader("src/headers/pch.hpp")
pchsource("src/controller/pch.cpp")
excludes({ "src/headers/*.gch" })
-- Platform-specific settings
filter("system:macosx")
libdirs({ "/opt/homebrew/Cellar/sfml/3.0.1/lib" })
links({
	"sfml-graphics",
	"sfml-window",
	"sfml-system",
	"OpenGL.framework",
})

filter("configurations:Debug")
defines({ "DEBUG" })
symbols("On")

filter("configurations:Release")
defines({ "NDEBUG" })
optimize("On")

filter({})
