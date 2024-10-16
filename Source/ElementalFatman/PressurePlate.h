// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerSupply.h"
#include "Curves/CurveFloat.h"
#include "PressurePlate.generated.h"

UCLASS()
class ELEMENTALFATMAN_API APressurePlate : public APowerSupply
{
	GENERATED_BODY()
	
public:	

	UFUNCTION(BlueprintCallable)
	void Press();

	UFUNCTION(BlueprintCallable)
	void Reset();

protected:

	void Setup();
	void InvokeSpecificMechanic();

	UFUNCTION()
	void Move(bool pressed);

	UPROPERTY(EditAnywhere)
	float PressDuration = 0.25f;

	// How far the pressure plate should move on the Z axis when pushed down (local/relative, NOT world pos).
	UPROPERTY(EditAnywhere)
	float PressedPos = -15;

	UPROPERTY(EditAnywhere)
	UCurveFloat* PressCurve = nullptr;

	FTimerHandle PressHandle;
	float PressAlpha;
	float StartPos;
};
