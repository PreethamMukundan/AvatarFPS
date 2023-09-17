// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Avatar : ModuleRules
{
	public Avatar(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Slate",
            "SlateCore",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",
            "Paper2D",
            "AIModule",
            "NavigationSystem",
            "UMG"
        });
    }
}
