// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerSupply.h"
#include "Curves/CurveFloat.h"
#include "Fan.generated.h"

UENUM()
enum class EFanStartState : int8
{
	Clockwise = 0,
	Anticlockwise = 1,
};

UENUM()
enum class EFanLimitations : int8
{
	Normal = 0,
	SingleDirectionClockwise = 1,
	SingleDirectionAnticlockwise = 2,
	UnlimitedSpinning = 3,
};

UCLASS()
class ELEMENTALFATMAN_API AFan : public APowerSupply
{
	GENERATED_BODY()

protected:

	void Setup();
	void InvokeSpecificMechanic();

	UPROPERTY(EditAnywhere, Category = "Fan | Core")
	EFanStartState StartState = EFanStartState::Clockwise;

	// Does this fan turn off after its spin duration, shutting off power?
	UPROPERTY(EditAnywhere, Category = "Fan | Core")
	bool Timed;

	bool TimedOut = false;

	// Does this fan have any special conditions? (Normal fans can be spun in both directions once).
	UPROPERTY(EditAnywhere, Category = "Fan | Core")
	EFanLimitations FanType = EFanLimitations::Normal;

	UFUNCTION()
	void Spin(bool clockwise);

	void StopSpinning(bool clockwise);

	FTimerHandle SpinHandler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fan | Spin")
	int32 SpinSpeed = 6;

	// How fast (seconds) it takes for the fan to get up to full spin speed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fan | Spin")
	float AccelerationDuration = 0.5f;

	// How long (seconds) the fan spins for.
	UPROPERTY(EditAnywhere, Category = "Fan | Spin")
	float SpinDuration = 4.f;

	UPROPERTY(EditAnywhere, Category = "Fan | Internal")
	UCurveFloat* AccelerationCurve;

	float AccelerationAlpha = 0;
	float DecelerationAlpha = 1;
};
