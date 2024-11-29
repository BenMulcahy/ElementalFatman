// Fill out your copyright notice in the Description page of Project Settings.


#include "MetalGrate.h"

void AMetalGrate::Setup()
{
	if (OverrideMesh)
	{
		// replace mesh with variants here
	}

	UE_LOG(LogInteraction, Warning, TEXT("setting up metal grate"));

	ObjectType = EObjectType::OT_Metal_Grate;

	MaxInteractablePips = 1;
	CurrentInteractablePips = 1;

	Super::Setup();
}


void AMetalGrate::InvokeSpecificMechanic()
{
	switch (CurrentInteractablePips)
	{
	case 0: // brittle
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("grate frozen/brittle")));
		SetBreakable(true);
		PreventInteraction();
		break;
	case 1: // starting state
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("grate solid")));
		SetBreakable(false);
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Grate current pip value error!"));
		break;
	}
}