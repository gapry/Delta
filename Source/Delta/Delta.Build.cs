// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System;

public class Delta : ModuleRules
{
  private readonly string OS;
  private readonly string Arch;

  public Delta(ReadOnlyTargetRules Target) : base(Target)
  {
    PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

    PublicDependencyModuleNames.AddRange(new string[] {
      "Core",
      "CoreUObject",
      "Engine",
      "InputCore",
      "EnhancedInput",
      "HairStrandsCore",
      "Niagara",
      "ControlRig",
      "RigVM",
      "AudioExtensions",
      "MetasoundEngine",
      "MetasoundFrontend",
      "MetasoundGraphCore",
      "GeometryCollectionEngine",
      "UMG",
    });

    PrivateDependencyModuleNames.AddRange(new string[] { });

    OS = DetectOS(Target.Platform);
    Arch = DetectArch(Target.Platform);

    AddVcpkgDependencies();

    // Uncomment if you are using Slate UI
    // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

    // Uncomment if you are using online features
    // PrivateDependencyModuleNames.Add("OnlineSubsystem");

    // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
  }

  private string DetectOS(UnrealTargetPlatform platform)
  {
    if (platform == UnrealTargetPlatform.Win64)
    {
      return "windows";
    }
    else if (platform == UnrealTargetPlatform.Mac)
    {
      return "osx";
    }
    else if (platform == UnrealTargetPlatform.Linux)
    {
      return "linux";
    }
    else
    {
      return "unknown-os";
    }
  }

  private string DetectArch(UnrealTargetPlatform platform)
  {
    if (platform == UnrealTargetPlatform.Win64 || platform == UnrealTargetPlatform.Mac || platform == UnrealTargetPlatform.Linux)
    {
      return "x64";
    }
    else if (platform == UnrealTargetPlatform.LinuxArm64)
    {
      return "arm64";
    }
    else
    {
      return "unknown-arch";
    }
  }

  private void AddVcpkgDependencies()
  {
    const string VcpkgRootDir = "../../vcpkg";

    string VcpkgInstalledDir = Path.Combine(VcpkgRootDir, $"installed/{Arch}-{OS}");

    string VcpkgLibDir = Path.Combine(ModuleDirectory, VcpkgInstalledDir, "lib");
    string VcpkgBinDir = Path.Combine(ModuleDirectory, VcpkgInstalledDir, "bin");
    string VcpkgIncludeDir = Path.Combine(ModuleDirectory, VcpkgInstalledDir, "include");

    PublicIncludePaths.Add(VcpkgIncludeDir);

    Action<string> SetupLibrary = (string libraryName) =>
    {
      PublicAdditionalLibraries.Add(Path.Combine(VcpkgLibDir, $"{libraryName}.lib"));

      PublicDelayLoadDLLs.Add($"{libraryName}.dll");

      RuntimeDependencies.Add("$(BinaryOutputDir)/" + libraryName + ".dll", Path.Combine(VcpkgBinDir, $"{libraryName}.dll"));
    };

    SetupLibrary("fmt");
  }
}
