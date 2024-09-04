// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeatInteractable.h"
#include "WoodenBarricade.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTALFATMAN_API AWoodenBarricade : public AHeatInteractable
{
	GENERATED_BODY()

public:

	void TryInteract(bool heating, int32 currentPlayerPips, int32 maxPlayerPips);

protected:

	void BurnAway();
	void UpdateInteractable(int32 interactionType) override;
};
