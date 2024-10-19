// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeatInteractable.h"
#include "MetalGrate.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTALFATMAN_API AMetalGrate : public AHeatInteractable
{
	GENERATED_BODY()

protected:

	void Setup();
	void InvokeSpecificMechanic();
};
