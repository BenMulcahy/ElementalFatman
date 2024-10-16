// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeatInteractable.h"
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

	FTimerHandle ExpandHandler;
	float ExpandSize = 1.f;

	void Expand();

};
