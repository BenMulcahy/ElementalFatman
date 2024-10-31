// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerSupply.h"
#include "ClockRing.h"
#include "Clock.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTALFATMAN_API AClock : public APowerSupply
{
	GENERATED_BODY()
	
	TArray<AClockRing*> Rings;
	TArray<bool> RingStates;

	void Setup();

	UFUNCTION()
	void UpdateRotation(AClockRing* ChangedRing, FRotator NewRotation);
};
