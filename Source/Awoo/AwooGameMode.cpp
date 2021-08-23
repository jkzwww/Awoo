// Copyright Epic Games, Inc. All Rights Reserved.

#include "AwooGameMode.h"
#include "AwooHUD.h"
#include "AwooCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAwooGameMode::AAwooGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AAwooHUD::StaticClass();
}
