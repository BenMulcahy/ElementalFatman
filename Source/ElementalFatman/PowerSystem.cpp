// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerSystem.h"

// Sets default values
APowerSystem::APowerSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void APowerSystem::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < RequiredPowerStates.Num(); i++)
	{

	}

	for (APowerSupply* AttachedPowerSupply : PowerSuppliers)
	{
		CurrentPowerStates.Add(AttachedPowerSupply->GetCurrentInteractablePips()); // make a list of their current heat/cool states
	}
}

void APowerSystem::UpdatePowerState(int32 NewState, int32 ChildNum)
{
	CurrentPowerStates[ChildNum] = NewState;

	//if (CheckPowerStates()) /*turn on powered objects*/ 
}

bool APowerSystem::CheckPowerStates()
{
	int CorrectPowerStates = 0;
	//for (int i : CurrentPowerStates)
	//{
	//	if (CurrentPowerStates[i] == RequiredPowerStates[i]) CorrectPowerStates++;
	//}

	if (CorrectPowerStates == RequiredPowerStates.Num()) return true;
	else return false;
}

