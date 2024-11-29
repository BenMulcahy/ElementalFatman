// Fill out your copyright notice in the Description page of Project Settings.


#include "HeatSource.h"

void AHeatSource::Setup()
{

	if (!OverrideMesh)
	{
		// construct mesh here
	}

	ObjectType = EObjectType::OT_Heat_Source;

	UE_LOG(LogInteraction, Warning, TEXT("setting up heat source"));

	MaxInteractablePips = 1;
	CurrentInteractablePips = 1;

	Super::Setup();

}


void AHeatSource::InvokeSpecificMechanic()
{
	switch (CurrentInteractablePips)
	{
	case 0:
		// probably change visuals
		break;
	case 1:
		// do nothing atm, can you re-heat heat sources?
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Heat source current pip value error!"));
		break;
	}
}