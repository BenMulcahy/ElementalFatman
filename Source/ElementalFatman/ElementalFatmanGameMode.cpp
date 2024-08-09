// Copyright Epic Games, Inc. All Rights Reserved.

#include "ElementalFatmanGameMode.h"
#include "ElementalFatmanCharacter.h"
#include "UObject/ConstructorHelpers.h"

AElementalFatmanGameMode::AElementalFatmanGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
