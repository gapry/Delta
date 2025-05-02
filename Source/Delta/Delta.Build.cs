// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System;

public class Delta : ModuleRules
{
  public Delta(ReadOnlyTargetRules Target) : base(Target)
  {
    PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

    PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

    PrivateDependencyModuleNames.AddRange(new string[] { });

    AddVcpkgDependencies();

    // Uncomment if you are using Slate UI
    // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

    // Uncomment if you are using online features
    // PrivateDependencyModuleNames.Add("OnlineSubsystem");

    // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
  }

  private void AddVcpkgDependencies()
  {
    string VcpkgLibDir = Path.Combine(ModuleDirectory, "../../vcpkg/installed/x64-windows/lib");
    string VcpkgBinDir = Path.Combine(ModuleDirectory, "../../vcpkg/installed/x64-windows/bin");

    PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "../../vcpkg/installed/x64-windows/include"));

    Action<string> SetupLibrary = (string libraryName) =>
    {
      PublicAdditionalLibraries.Add(Path.Combine(VcpkgLibDir, $"{libraryName}.lib"));

      PublicDelayLoadDLLs.Add($"{libraryName}.dll");

      RuntimeDependencies.Add("$(BinaryOutputDir)/" + libraryName + ".dll", Path.Combine(VcpkgBinDir, $"{libraryName}.dll"));
    };

    SetupLibrary("fmt");
  }
}
