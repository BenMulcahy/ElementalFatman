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

protected:

	void Setup();
	void InvokeSpecificMechanic();

};
