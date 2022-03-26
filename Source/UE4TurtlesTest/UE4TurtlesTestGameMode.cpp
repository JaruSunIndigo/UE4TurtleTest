// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE4TurtlesTestGameMode.h"
#include "UE4TurtlesTestHUD.h"
#include "UE4TurtlesTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUE4TurtlesTestGameMode::AUE4TurtlesTestGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AUE4TurtlesTestHUD::StaticClass();
}
