// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RhythmGunner : ModuleRules
{
	public RhythmGunner(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG" });
	}
}
