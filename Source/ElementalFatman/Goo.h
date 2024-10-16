// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeatInteractable.h"
#include "Curves/CurveVector.h"
#include "Goo.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTALFATMAN_API AGoo : public AHeatInteractable
{
	GENERATED_BODY()

protected:

	void Setup();
	void InvokeSpecificMechanic();

	void Shrink();
	void StopShrinking();

	FTimerHandle ShrinkHandler;

	UPROPERTY(EditAnywhere)
	float ShrinkDuration;

	FVector StartSize;
	UPROPERTY(EditAnywhere)
	float ShrinkSize;

	UPROPERTY(EditAnywhere)
	UCurveVector* ShrinkCurve = nullptr;

	float ShrinkAlpha = 0;

	FVector BoxExtents;
};
