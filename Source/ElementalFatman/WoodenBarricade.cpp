// Fill out your copyright notice in the Description page of Project Settings.


#include "WoodenBarricade.h"

void AWoodenBarricade::Setup() 
{
	if (!OverrideMesh) 
	{
		// construct mesh here
	}

	ObjectType = EObjectType::OT_Barricade;

	UE_LOG(LogInteraction, Warning, TEXT("setting up barricade"));

	MaxInteractablePips = 1;
	CurrentInteractablePips = 0;
}


void AWoodenBarricade::InvokeSpecificMechanic(int32 interactionType) 
{
	UE_LOG(LogTemp, Warning, TEXT("SUBCLASS INVOKING!!!"))

	if (interactionType < 0) 
	{
		// set timer, play animation
		UE_LOG(LogInteraction, Warning, TEXT("destroying"));
		this->Destroy();
	}
}