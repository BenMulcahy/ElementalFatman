// Fill out your copyright notice in the Description page of Project Settings.


#include "WoodenBarricade.h"

void AWoodenBarricade::TryInteract(bool heating, int32 currentPlayerPips, int32 maxPlayerPips) 
{
	int32 interactionType = ValidateInteraction(heating, currentPlayerPips, maxPlayerPips);
	UpdateInteractable(interactionType);
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
	this->Destroy();
}