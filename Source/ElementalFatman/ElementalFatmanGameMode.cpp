// Copyright Epic Games, Inc. All Rights Reserved.

#include "ElementalFatmanGameMode.h"
#include "ElementalFatmanCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "HeatInteractable.h"

AElementalFatmanGameMode::AElementalFatmanGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void AElementalFatmanGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetSceneInteractables(FindSceneInteractables());
	UE_LOG(LogGameMode, Warning, TEXT("set: %d interactables"), GetSceneInteractables().Num());
}


/// <summary>
/// finds all scene interactables
/// </summary>
/// <returns>array of aactors</returns>
TArray<AActor*> AElementalFatmanGameMode::FindSceneInteractables() 
{
	TArray<AActor*> tempInteractables = TArray<AActor*>();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHeatInteractable::StaticClass(), tempInteractables);

	//UE_LOG(LogGameMode, Log, TEXT("found: %d temp interactables"), tempInteractables.Num());
	//for (AActor* sceneActor : tempInteractables) UE_LOG(LogGameMode, Log, TEXT("found: %s"), *sceneActor->GetName());

	return tempInteractables;
}
