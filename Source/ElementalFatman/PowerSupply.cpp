// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerSupply.h"

// Sets default values
APowerSupply::APowerSupply()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void APowerSupply::BeginPlay()
{
	Super::BeginPlay();

	for (AActor* AttachedActor : PowerSuppliers)
	{
		AHeatInteractable* AttachedHeatInteractable = nullptr;

		// checks to ensure the assigned power suppliers are valid (idiot designer proofing)
		if (Cast<AHeatInteractable>(AttachedActor)) AttachedHeatInteractable = Cast<AHeatInteractable>(AttachedActor);
		else UE_LOG(LogTemp, Warning, TEXT("%s is not a heat interactable!"), AttachedActor->GetName()); 

		if (AttachedHeatInteractable->GetIsPowerSupply()) AttachedHeatInteractables.Add(AttachedHeatInteractable);
		else UE_LOG(LogTemp, Warning, TEXT("%s is not a power supplier!"), *UEnum::GetDisplayValueAsText(AttachedHeatInteractable->GetObjectType()).ToString());
	}

	for (int i = 0; i < RequiredPowerStates.Num(); i++)
	{

	}

	for (AHeatInteractable* AttachedPowerSupply : AttachedHeatInteractables)
	{
		CurrentPowerStates.Add(AttachedPowerSupply->GetCurrentInteractablePips()); // make a list of their current heat/cool states
	}
}

void APowerSupply::UpdatePowerState(int32 NewState, int32 ChildNum) 
{
	CurrentPowerStates[ChildNum] = NewState;

	//if (CheckPowerStates()) /*turn on powered objects*/ 
}

bool APowerSupply::CheckPowerStates() 
{
	int CorrectPowerStates = 0;
	//for (int i : CurrentPowerStates)
	//{
	//	if (CurrentPowerStates[i] == RequiredPowerStates[i]) CorrectPowerStates++;
	//}

	if (CorrectPowerStates == RequiredPowerStates.Num()) return true;
	else return false;
}

