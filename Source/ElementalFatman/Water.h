// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeatInteractable.h"
#include "Water.generated.h"

UENUM()
enum class EWaterState : uint8 
{
	Ice = 0,
	Water = 1,
	Steam = 2
};

/**
 * 
 */
UCLASS()
class ELEMENTALFATMAN_API AWater : public AHeatInteractable
{
	GENERATED_BODY()

protected:

	void Setup();
	void InvokeSpecificMechanic();

	UPROPERTY(EditAnywhere)
	EWaterState StartAs = EWaterState::Water;

};
