// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FantasyWorld : ModuleRules
{
	public FantasyWorld(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "GeometryCollectionEngine", "Niagara", "UMG", "AIModule", "MediaAssets", "SlateCore" });
	}
}
