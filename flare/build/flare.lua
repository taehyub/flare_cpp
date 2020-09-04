project "flare"
	kind "StaticLib"
	language "C++"

	configuration { "Debug" }
		targetdir "./debug/lib"
		objdir "./debug/obj"

	configuration { "Release" }
		targetdir "./release/lib"
		objdir "./release/obj"

	configuration { }

	includedirs 
	{
		"../include",
		"../../flare_math/include"
	}

	buildoptions_cpp 
	{
		"-std=c++17",
	}
   
	files
	{
		"../src/**.cpp"	
	}

	targetname "flare"