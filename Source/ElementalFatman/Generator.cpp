// Fill out your copyright notice in the Description page of Project Settings.


#include "Generator.h"

void AGenerator::Setup()
{
	if (OverrideMesh)
	{
		// replace mesh with variants here
	}

	UE_LOG(LogInteraction, Warning, TEXT("setting up generator"));

	ObjectType = EObjectType::OT_Generator;

	MaxInteractablePips = 1;
	CurrentInteractablePips = 0;	
	
	Super::Setup();
	InvokeSpecificMechanic();
}


void AGenerator::InvokeSpecificMechanic()
{
	switch (CurrentInteractablePips)
	{
		case 0: // switch off
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("generator turned off")));
			break;
		case 1: // switch on
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("generator turned on")));
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("Generator current pip value error!"));
			break;
	}

	PowerStateChangedDelegate.Broadcast(this, CurrentInteractablePips);
}