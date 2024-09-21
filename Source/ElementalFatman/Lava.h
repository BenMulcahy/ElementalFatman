// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeatInteractable.h"
#include "Lava.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTALFATMAN_API ALava : public AHeatInteractable
{
	GENERATED_BODY()

protected:
	
	ALava();

	void Setup();
	void InvokeSpecificMechanic();

};
