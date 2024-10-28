// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingMechanism.h"

void AMovingMechanism::Setup()
{
	if (OverrideMesh)
	{
		// replace mesh with variants here
	}

	UE_LOG(LogInteraction, Warning, TEXT("setting up moving mechanism"));

	ObjectType = EObjectType::OT_Mechanism;

	MaxInteractablePips = 1;
	CurrentInteractablePips = StartOn ? 1 : 0;

	Super::Setup();
}


void AMovingMechanism::InvokeSpecificMechanic()
{
	switch (CurrentInteractablePips)
	{
	case 0: // switch off
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("mechanism turned off")));
		break;
	case 1: // switch on
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("mechanism turned on")));
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Mechanism current pip value error!"));
		break;
	}

	PowerStateChangedDelegate.Broadcast(this, CurrentInteractablePips);
}