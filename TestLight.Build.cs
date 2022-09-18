// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class TestLight : ModuleRules
{
	public TestLight(ReadOnlyTargetRules target) : base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"UMG",
			"OnlineSubsystem",
			"OnlineSubsystemSteam",
		});
	}
}
