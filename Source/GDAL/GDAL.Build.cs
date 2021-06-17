// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class GDAL : ModuleRules
{
	public GDAL(ReadOnlyTargetRules Target) : base(Target)
	{
		bEnableUndefinedIdentifierWarnings = false;
        PCHUsage = PCHUsageMode.NoSharedPCHs;
        PrivatePCHHeaderFile = "Public/GDALModule.h";
        bUseUnity = false;
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
				// ... add other public dependencies that you statically link with here ...
			}
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Engine",
                "Projects",
				// ... add private dependencies that you statically link with here ...	
			}
            );


        string GDALPath = ModuleDirectory;
        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            string PlatformSubPath = (Target.Platform == UnrealTargetPlatform.Win64) ? "Win64" : "Win32";
            string LibrariesPath = Path.Combine(GDALPath, "lib");
            string DLLPath = Path.Combine(GDALPath, "dll");

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, PlatformSubPath, "gdal_i.lib"));
            //PublicLibraryPaths.Add(Path.Combine(LibrariesPath, PlatformSubPath));
            //PublicLibraryPaths.Add(Path.Combine(DLLPath, PlatformSubPath));

            string IncludePath = Path.Combine(GDALPath, "include");
            PublicIncludePaths.Add(IncludePath);

            PublicDelayLoadDLLs.Add("gdal300.dll");
            PublicDelayLoadDLLs.Add("libpq.dll");
            PublicDelayLoadDLLs.Add("libintl-8.dll");
            PublicDelayLoadDLLs.Add("libiconv-2.dll");
            PublicDelayLoadDLLs.Add("libeay32.dll");
            PublicDelayLoadDLLs.Add("ssleay32.dll");

            RuntimeDependencies.Add(Path.Combine(DLLPath, PlatformSubPath, "gdal300.dll"));
            RuntimeDependencies.Add(Path.Combine(DLLPath, PlatformSubPath, "libpq.dll"));
            RuntimeDependencies.Add(Path.Combine(DLLPath, PlatformSubPath, "libintl-8.dll"));
            RuntimeDependencies.Add(Path.Combine(DLLPath, PlatformSubPath, "libiconv-2.dll"));
            RuntimeDependencies.Add(Path.Combine(DLLPath, PlatformSubPath, "libeay32.dll"));
            RuntimeDependencies.Add(Path.Combine(DLLPath, PlatformSubPath, "ssleay32.dll"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            string SDKPath = Path.Combine(GDALPath, "Linux");
            PublicIncludePaths.Add(Path.Combine(SDKPath, "include"));
            PublicAdditionalLibraries.Add(Path.Combine(SDKPath, "lib", "libgdal.so"));
			PublicAdditionalLibraries.Add(Path.Combine(SDKPath, "lib", "libproj.so"));

			RuntimeDependencies.Add(Path.Combine(SDKPath, "lib", "libgdal.so"));
			RuntimeDependencies.Add(Path.Combine(SDKPath, "lib", "libproj.so"));

		}

        
        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
            );
    }
}
