// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE4TurtlesTest : ModuleRules
{
	public UE4TurtlesTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
