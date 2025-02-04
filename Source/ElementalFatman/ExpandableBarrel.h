// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeatInteractable.h"
#include "Curves/CurveFloat.h"
#include "ExpandableBarrel.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTALFATMAN_API AExpandableBarrel : public AHeatInteractable
{
	GENERATED_BODY()
	
protected:

	void Setup();
	void InvokeSpecificMechanic();
	
	void Expand();

	FTimerHandle ExpandHandler;

	UPROPERTY(EditAnywhere)
	float ExpandSize = 1.f;

	UPROPERTY(EditAnywhere)
	float ExpandDuration;

	UPROPERTY(EditAnywhere)
	UCurveFloat* ExpandCurve = nullptr;

	float ExpandAlpha = 0;
	float StartSize;
};
