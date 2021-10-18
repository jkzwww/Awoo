// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Awoo : ModuleRules
{
	public Awoo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay","NavigationSystem","GameplayTasks", "Niagara" });

		PublicIncludePaths.AddRange(new string[] { "Awoo/",
			"C:/Program Files (x86)/Epic Games/UE_4.26/Engine/Plugins/FX/Niagara/Source/Niagara/Public",
			"C:/Program Files (x86)/Epic Games/UE_4.26/Engine/Plugins/FX/Niagara/Source/Niagara/Classes",
			"C:/Program Files (x86)/Epic Games/UE_4.26/Engine/Plugins/FX/Niagara/Source/Niagara/Private"});
	}
}
