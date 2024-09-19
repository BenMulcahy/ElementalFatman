// Fill out your copyright notice in the Description page of Project Settings.


#include "Generator.h"

void AGenerator::Setup()
{
	if (!OverrideMesh)
	{
		// construct mesh here
	}

	ObjectType = EObjectType::OT_Barricade;

	UE_LOG(LogInteraction, Warning, TEXT("setting up generator"));

	MaxInteractablePips = 1;
	CurrentInteractablePips = 0;
}


void AGenerator::InvokeSpecificMechanic(int32 interactionType)
{
	if (interactionType < 1)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("generator turned off")));
	}

	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("generator turned on")));
		if (Door) Cast<ADoor>(Door)->Open();
	}
}