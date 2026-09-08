// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TMS_LS : ModuleRules
{
	public TMS_LS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"GameplayAbilities",
			"GameplayTasks",
			"GameplayTags",
			"AnimGraphRuntime",
			"MotionWarping",
			"UMG",
            "DeveloperSettings",
            "AIModule",
            "NavigationSystem", 
			"Niagara",
			"NiagaraCore",
			"PhysicsCore",
			"Json",
			"JsonUtilities",
			"TMS_LoadingScreen",
			"Slate",
			"SlateCore",
		});
	}
}
