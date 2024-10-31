// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveFloat.h"
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
	void Melt();
	void StopMelting();

	UPROPERTY(EditAnywhere, Category = "Water | Core")
	EWaterState StartAs = EWaterState::Water;

	UPROPERTY(EditAnywhere, Category = "Water | Move When Melted")
	FVector StartScale;	

	UPROPERTY(EditAnywhere, Category = "Water | Move When Melted")
	FVector StartLocation;
	
	UPROPERTY(EditAnywhere, Category = "Water | Move When Melted")
	FVector EndScale;	
	
	UPROPERTY(EditAnywhere, Category = "Water | Move When Melted")
	FVector EndLocation;

	// How long does it take (seconds) for water to shift scale/location when melting.
	UPROPERTY(EditAnywhere, Category = "Water | Move When Melted")
	float MeltDuration = 1;

	UPROPERTY(EditAnywhere, Category = Internal)
	UCurveFloat* WaterCurve = nullptr;

	FTimerHandle MeltHandle;
	float MeltAlpha = 0;
	bool Melted = false;
};
