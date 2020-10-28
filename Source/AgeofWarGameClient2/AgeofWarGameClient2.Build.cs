// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class AgeofWarGameClient2 : ModuleRules
{
	public AgeofWarGameClient2(ReadOnlyTargetRules Target) : base(Target)
	{
        PrivatePCHHeaderFile = "AgeofWarGameClient2.h";

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "AIModule", "Http", "Json", "JsonUtilities", "Sockets", "Networking", "GameplayTasks" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "Paper2D" });

        PublicDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemUtils", "Networking", "Sockets" });

        PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "Networking", "Sockets" });

        PrivateDependencyModuleNames.Add("OnlineSubsystem");

        if ((Target.Platform == UnrealTargetPlatform.Android) || (Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
        {
            DynamicallyLoadedModuleNames.Add("AndroidAdvertising");
            DynamicallyLoadedModuleNames.Add("OnlineSubsystemGooglePlay");
        }

        // �¶��� ����� ����� ��� �ּ��� ���� �ϸ�� 
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
