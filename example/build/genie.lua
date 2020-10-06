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

	configuration { }
	
	libdirs 
	{ 
		"../../dependencies/glfw_build/src",
		"../../dependencies/skia/out/Static",
                "/usr/local/lib/x86_64-linux-gnu"
	}

	includedirs 
	{
                "/usr/local/include/elementary-1",
                "/usr/local/include/efl-1",
                "/usr/local/include/eina-1",
                "/usr/local/include/eina-1/eina",
                "/usr/local/include/eet-1",
                "/usr/local/include/emile-1",
                "/usr/local/include/evas-1",
                "/usr/local/include/eo-1",
                "/usr/local/include/ecore-1",
                "/usr/local/include/ecore-evas-1",
                "/usr/local/include/ecore-file-1",
                "/usr/local/include/ecore-input-1",
                "/usr/local/include/ecore-imf-1",
                "/usr/local/include/ecore-con-1",
                "/usr/local/include/edje-1",
                "/usr/local/include/eldbus-1",
                "/usr/local/include/efreet-1",
                "/usr/local/include/ethumb-client-1",
                "/usr/local/include/ethumb-1",
                "/usr/local/include/edje-1",
                "/usr/local/include/eldbus-1",
                "/usr/local/include/efreet-1",
                "/usr/local/include/ethumb-client-1",
                "/usr/local/include/ethumb-1",
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
		"GL",
		"ecore",
		"evas",
		"elementary",
		"thorvg"
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
