// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerSupply.h"
#include "Curves/CurveFloat.h"
#include "ThreeWaySwitch.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSwitchSpinning, AThreeWaySwitch*, SpinningSwitch, bool, Direction);

UENUM()
enum class ESwitchStartState : int8
{
	Left = 0,
	Neutral = 1,
	Right = 2,
};

UCLASS()
class ELEMENTALFATMAN_API AThreeWaySwitch : public APowerSupply
{
	GENERATED_BODY()

public:

	FOnSwitchSpinning SwitchSpinningDelegate;

protected:

	void Setup();
	void InvokeSpecificMechanic();

	UPROPERTY(EditAnywhere, Category = "Switch | Core")
	ESwitchStartState StartState = ESwitchStartState::Neutral;

	float LastState;

	UFUNCTION()
	void Spin(bool clockwise);

	void StopSpinning(bool clockwise);

	FTimerHandle SpinHandler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch | Spin")
	int32 SpinSpeed = 6;

	// How fast (seconds) it takes for the fan to get up to full spin speed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch | Spin")
	float AccelerationDuration = 0.5f;

	// How long (seconds) the fan spins for.
	UPROPERTY(EditAnywhere, Category = "Switch | Spin")
	float SpinDuration = 2.f;

	UPROPERTY(EditAnywhere, Category = "Switch | Internal")
	UCurveFloat* AccelerationCurve;

	float AccelerationAlpha = 0;
	float DecelerationAlpha = 1;
};
