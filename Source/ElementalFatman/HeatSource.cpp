// Fill out your copyright notice in the Description page of Project Settings.


#include "HeatSource.h"

void AHeatSource::Setup()
{
	if (!OverrideMesh)
	{
		// construct mesh here
	}

	ObjectType = EObjectType::OT_Barricade;

	UE_LOG(LogInteraction, Warning, TEXT("setting up heat source"));

	MaxInteractablePips = 1;
	CurrentInteractablePips = 1;
}


void AHeatSource::InvokeSpecificMechanic(int32 interactionType)
{

}