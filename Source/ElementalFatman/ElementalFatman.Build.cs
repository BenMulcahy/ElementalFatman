// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ElementalFatman : ModuleRules
{
	public ElementalFatman(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "HeadMountedDisplay", "UMG", "PhysicsCore" });
	}
}
