// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Fan.h"
#include "Curves/CurveFloat.h"
#include "ClockRing.generated.h"

USTRUCT()
struct FFanInstance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	AFan* Fan = nullptr;

	// The angle this fan will turn the ring each time it is heated/cooled (degrees).
	UPROPERTY(EditAnywhere, meta = (Units = "Degrees", Delta = 5))
	float TurnAngle;

	// Reverse the direction this ring will turn when the fan is spun (e.g. spinning the fan clockwise turns the ring anticlockwise).
	UPROPERTY(EditAnywhere)
	bool ReverseDirection = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRotationChanged, AClockRing*, ChangedRing, FRotator, NewRotation);

UCLASS()
class ELEMENTALFATMAN_API AClockRing : public AActor
{
	GENERATED_BODY()

public:
	FOnRotationChanged RotationChangedDelegate;

protected:

	// What "level" this ring is (starts at 1).
	UPROPERTY(EditAnywhere, Category = "Clock | Ring Setup")
	int RingNumber = 1;

	UPROPERTY(EditAnywhere, Category = "Clock | Ring Setup", meta=(Units="Degrees", Delta = 5))
	float StartAngle;

	// How quickly the ring turns.
	UPROPERTY(EditAnywhere, Category = "Clock | Ring Turning")
	float TurnSpeed = 1.f;

	UPROPERTY(EditAnywhere, Category = "Clock | Ring Turning")
	TArray<FFanInstance> Fans;

	virtual void BeginPlay() override;
	UFUNCTION()
	void ChangeRotation(APowerSupply* SpinningFan, int32 SpinDirection);
	
	UFUNCTION()
	void Turn(float NewYaw);
	void StopTurning();

	UPROPERTY(EditAnywhere, Category = "Clock | Internal")
	UCurveFloat* ClockCurve;
	FTimerHandle TurnHandler;
	float TurnAlpha;
	FRotator LastRotation;
};
