// Fill out your copyright notice in the Description page of Project Settings.


#include "WoodenBarricade.h"

int32 AWoodenBarricade::ValidateInteraction(bool heating, int32 currentPlayerPips, int32 maxPlayerPips) 
{
	Super::ValidateInteraction(heating, currentPlayerPips, maxPlayerPips);

	return 1;
}

void AWoodenBarricade::UpdateInteractable(int32 interactionType)
{
	Super::UpdateInteractable(interactionType);

	// heating
	if (interactionType < 0) BurnAway();
}

void AWoodenBarricade::BurnAway() 
{
	// set timer, play animation
	UE_LOG(LogInteraction, Warning, TEXT("destroying"));
	this->Destroy();
}