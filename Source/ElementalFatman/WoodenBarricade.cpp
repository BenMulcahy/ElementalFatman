// Fill out your copyright notice in the Description page of Project Settings.


#include "WoodenBarricade.h"

void AWoodenBarricade::Setup() 
{
	Super::Setup();

	if (!OverrideMesh) 
	{
		// construct mesh here
	}

	ObjectType = EObjectType::OT_Wood_Barricade;

	UE_LOG(LogInteraction, Warning, TEXT("setting up barricade"));

	MaxInteractablePips = 1;
	CurrentInteractablePips = 0;
}


void AWoodenBarricade::InvokeSpecificMechanic() 
{
	switch (CurrentInteractablePips)
	{
	case 0:
		// do nothing but don't throw error msg
		break;
	case 1:
		UE_LOG(LogInteraction, Warning, TEXT("destroying"));
		this->Destroy();
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Barricade current pip value error!"));
		break;
	}
}