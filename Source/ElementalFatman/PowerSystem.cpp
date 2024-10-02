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

	if (!AreEntriesValid()) UE_LOG(LogTemp, Error, TEXT("Invalid power system!!!"));

	// make a list of each power supply's current heat/cool state
	for (APowerSupply* PowerSupply : PowerSuppliers) CurrentPowerStates.Add(PowerSupply->GetCurrentInteractablePips());

	// subscribe to each power supply's delegate, which broadcasts whenever the power supply is heated or cooled and returns its pip value
	for (APowerSupply* PowerSupply : PowerSuppliers)
	{
		PowerSupply->PowerStateChangedDelegate.AddUniqueDynamic(this, &APowerSystem::UpdatePowerState);
	}
}

bool APowerSystem::AreEntriesValid() 
{
	// power system is only valid if there is:
	// at least one supplier
	if (PowerSuppliers.Num() == 0) return false;
	// at least one receiver
	if (PowerReceivers.Num() == 0) return false;
	// an equal number of suppliers and required supplier states
	if (RequiredPowerStates.Num() != PowerSuppliers.Num()) return false;
	return true;
}

void APowerSystem::UpdatePowerState(APowerSupply* UpdatedPowerSupply, int32 NewPowerState)
{	
	
	UE_LOG(LogTemp, Warning, TEXT("called by %s"), *UpdatedPowerSupply->GetName());

	for (int i = 0; i < PowerSuppliers.Num(); i++) 
	{
		if (PowerSuppliers[i] == UpdatedPowerSupply)
		{
			UE_LOG(LogTemp, Warning, TEXT("found correct updated supply"));
			CurrentPowerStates[i] = NewPowerState;

			UE_LOG(LogTemp, Warning, TEXT("desired state: %d, new state: %d"), RequiredPowerStates[i], CurrentPowerStates[i]);
		}
	}


	for (APowerReceiver* PowerReceiver : PowerReceivers) PowerReceiver->Power(IsPowerSupplied());
}

bool APowerSystem::IsPowerSupplied()
{
	int CorrectPowerStates = 0;
	for (int i = 0; i < CurrentPowerStates.Num(); i++)
	{
		if (CurrentPowerStates[i] == (int)RequiredPowerStates[i]) CorrectPowerStates++;
	}

	if (CorrectPowerStates == RequiredPowerStates.Num()) return true;
	else return false;
}

