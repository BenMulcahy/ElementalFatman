// Fill out your copyright notice in the Description page of Project Settings.


#include "Generator.h"

void AGenerator::Setup()
{
	Super::Setup();

	if (OverrideMesh)
	{
		// replace mesh with variants here
	}

	UE_LOG(LogInteraction, Warning, TEXT("setting up generator"));

	ObjectType = EObjectType::OT_Generator;

	MaxInteractablePips = 1;
	CurrentInteractablePips = 0;
}


void AGenerator::InvokeSpecificMechanic()
{
	switch (CurrentInteractablePips)
	{
		case 0: // switch off
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("generator turned off")));
			if (Door) Cast<ADoor>(Door)->Close();
			SwitchedOn = false;
			break;
		case 1: // switch on
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("generator turned on")));
			if (Door) Cast<ADoor>(Door)->Open();
			SwitchedOn = true;
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("Generator current pip value error!"));
			break;
	}
}