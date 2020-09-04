BASE_DIR = path.getabsolute("../..")

solution "example"

	configurations
	{
		"Debug",
		"Release"
	}

configuration "Debug"
	flags
	{
		"Symbols"
	}

location("./");
dofile(path.join(BASE_DIR, "flare_math/build/flare_math.lua"))
location("./");
dofile(path.join(BASE_DIR, "flare/build/flare.lua"))
location("./");
dofile(path.join(BASE_DIR, "flare_skia/build/flare_skia.lua"))

project "example"
	kind "ConsoleApp"
	language "C++"

	configuration { "Debug" }
		targetdir "./debug/bin"
		objdir "./debug/bin/obj"

	configuration { "Release" }
		targetdir "./release/bin"
		objdir "./release/bin/obj"

	configuration "macosx"
		includedirs { "/usr/local/include", "/usr/include/" }
		libdirs { "/usr/local/lib", "/usr/lib/x86_64-linux-gnu" }

	configuration { }
	
	libdirs 
	{ 
		"../../dependencies/glfw_build/src",
		"../../dependencies/skia/out/Static",
                "/usr/lib/x86_64-linux-gnu"
	}

	includedirs 
	{
		"../../dependencies/glfw/include",
		"../../dependencies/skia",
		"../../dependencies/skia/include/core",
		"../../dependencies/skia/include/effects",
		"../../dependencies/skia/include/gpu",
		"../../dependencies/skia/include/config",
		"../../flare_skia/include",
		"../../flare/include",
		"../../flare_math/include"
	}

	links 
	{
		"flare_skia",
		"flare", 
		"flare_math",
		"glfw3",
		"skia",
		"png",
		"zlib",
		"sksg",
		"skshaper",
		"freetype2",
		"jpeg",
		"webp",
		"webpdemux",
		"webpmux",
		"pthread",
		"dl",
		"GL"
	}

	buildoptions_cpp 
	{
		"-std=c++17",
	}
   
	files
	{
		"../src/**.cpp"	
	}

	targetname "example"
