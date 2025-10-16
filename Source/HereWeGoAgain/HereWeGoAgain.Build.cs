// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HereWeGoAgain : ModuleRules
{
	public HereWeGoAgain(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"HereWeGoAgain",
			"HereWeGoAgain/Variant_Horror",
			"HereWeGoAgain/Variant_Horror/UI",
			"HereWeGoAgain/Variant_Shooter",
			"HereWeGoAgain/Variant_Shooter/AI",
			"HereWeGoAgain/Variant_Shooter/UI",
			"HereWeGoAgain/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
