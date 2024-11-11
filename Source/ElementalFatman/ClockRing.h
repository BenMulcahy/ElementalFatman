// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThreeWaySwitch.h"
#include "PressurePlate.h"
#include "Curves/CurveFloat.h"
#include "ClockRing.generated.h"

USTRUCT()
struct FSwitchInstance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	AThreeWaySwitch* Switch = nullptr;

	// The angle this fan will turn the ring each time it is heated/cooled (degrees).
	UPROPERTY(EditAnywhere, meta = (Units = "Degrees", Delta = 5))
	float TurnAngle;

	// Reverse the direction this ring will turn when the fan is spun (e.g. spinning the fan clockwise turns the ring anticlockwise).
	UPROPERTY(EditAnywhere)
	bool ReverseDirection = false;

	// Can this switch be disconnected from the clock ring (spin without turning the ring) by standing on a pressure plate?
	UPROPERTY(EditAnywhere)
	bool ControlledByPressurePlate = false;

	// Reference a pressure plate that can be used to disconnect this switch from the clock ring.
	UPROPERTY(EditAnywhere, meta = (EditCondition = "ControlledByPressurePlate", EditConditionHides))
	APressurePlate* PressurePlate = nullptr;

	bool SwitchDisconnected = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRotationChanged, AClockRing*, ChangedRing, FRotator, NewRotation);

UCLASS()
class ELEMENTALFATMAN_API AClockRing : public AActor
{
	GENERATED_BODY()

public:
	FOnRotationChanged RotationChangedDelegate;

protected:

	AClockRing();

	// What "level" this ring is (starts at 1).
	UPROPERTY(EditAnywhere, Category = "Clock | Ring Setup")
	int RingNumber = 1;

	UPROPERTY(EditAnywhere, Category = "Clock | Ring Setup", meta=(Units="Degrees", Delta = 5))
	float StartAngle;

	UPROPERTY(EditAnywhere, Category = "Clock | Ring Setup")
	bool ShowDebugLine = true;

	// How quickly the ring turns.
	UPROPERTY(EditAnywhere, Category = "Clock | Ring Turning")
	float TurnSpeed = 1.f;

	UPROPERTY(EditAnywhere, Category = "Clock | Ring Turning")
	TArray<FSwitchInstance> Switches;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void SetupPressurePlates();

	UFUNCTION()
	void DisconnectSwitch(APowerSupply* UpdatedPressurePlate, int32 NewPipState);

	UFUNCTION()
	void ChangeRotation(AThreeWaySwitch* ChangedSwitch, bool SpinDirection);
	
	UFUNCTION()
	void Turn(float NewYaw);
	void StopTurning();

	UPROPERTY(EditAnywhere, Category = "Clock | Internal")
	UCurveFloat* ClockCurve;
	FTimerHandle TurnHandler;
	float TurnAlpha;
	FRotator LastRotation;
};
