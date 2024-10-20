// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerSupply.h"
#include "Curves/CurveFloat.h"
#include "Fan.generated.h"

UCLASS()
class ELEMENTALFATMAN_API AFan : public APowerSupply
{
	GENERATED_BODY()

protected:

	void Setup();
	void InvokeSpecificMechanic();

	// Does this fan turn off after its spin duration, shutting off power?
	UPROPERTY(EditAnywhere, Category = Mechanics)
	bool Timed;

	UFUNCTION()
	void Spin(bool clockwise);

	void StopSpinning();

	FTimerHandle SpinHandler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mechanics)
	int32 SpinSpeed = 6;

	// How fast (seconds) it takes for the fan to get up to full spin speed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mechanics)
	float AccelerationDuration = 0.5f;

	// How long (seconds) the fan spins for.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mechanics)
	float SpinDuration = 4.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mechanics)
	UCurveFloat* AccelerationCurve;

	float AccelerationAlpha = 0;
	float DecelerationAlpha = 1;

};
