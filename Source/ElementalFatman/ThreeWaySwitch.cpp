// Fill out your copyright notice in the Description page of Project Settings.


#include "ThreeWaySwitch.h"

void AThreeWaySwitch::Setup()
{
	if (!OverrideMesh)
	{
		// construct mesh here
	}

	ObjectType = EObjectType::OT_Switch;

	UE_LOG(LogInteraction, Warning, TEXT("setting up 3 way switch"));

	MaxInteractablePips = 2;
	CurrentInteractablePips = (int)StartState;
	LastState = (int)StartState;

	Super::Setup();
}


void AThreeWaySwitch::InvokeSpecificMechanic()
{
	FTimerDelegate SpinDelegate;
	// reset the lerp alphas to their original values
	AccelerationAlpha = 0;
	DecelerationAlpha = 1;

	GetWorld()->GetTimerManager().ClearTimer(SpinHandler);

	switch (CurrentInteractablePips)
	{
	case 0: // left
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("switch in leftmost state"), CurrentInteractablePips));
		SpinDelegate.BindUFunction(this, "Spin", true);
		SwitchSpinningDelegate.Broadcast(this, true);
		break;

	case 1: // neutral
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("switch in central state"), CurrentInteractablePips));
		SpinDelegate.BindUFunction(this, "Spin", LastState == 0 ? false : true);
		SwitchSpinningDelegate.Broadcast(this, LastState == 0 ? false : true);
		break;

	case 2: // right
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("switch in rightmost state"), CurrentInteractablePips));
		SpinDelegate.BindUFunction(this, "Spin", false);
		SwitchSpinningDelegate.Broadcast(this, false);
		break;

	default:
		UE_LOG(LogTemp, Error, TEXT("Switch current pip value error!"));
		break;
	}

	// prevent further interactions until fan is finished spinning
	PreventInteraction();

	// start the timer and broadcast the power state change
	GetWorld()->GetTimerManager().SetTimer(SpinHandler, SpinDelegate, GetWorld()->DeltaTimeSeconds, true);
	PowerStateChangedDelegate.Broadcast(this, CurrentInteractablePips);
}

void AThreeWaySwitch::Spin(bool clockwise)
{
	// finish spinning when the total spin duration is over
	if (DecelerationAlpha <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("total spin time: %f seconds"), AccelerationAlpha);
		StopSpinning(clockwise);
		return;
	}

	// start decelerating towards the end of the spin duration
	if (AccelerationAlpha >= SpinDuration - AccelerationDuration) DecelerationAlpha += GetWorld()->DeltaTimeSeconds / -AccelerationDuration;

	// start accelerating and keep track of the time the fan has been spinning
	AccelerationAlpha += GetWorld()->DeltaTimeSeconds;

	// take a point from the acceleration curve based on either the acceleration or deceleration alpha
	// once acceleration is complete the alpha will just be > 1 so it'll take the point at 1 (max speed) until deceleration happens
	float NormalizedNewSpeed = AccelerationCurve->GetFloatValue(AccelerationAlpha >= SpinDuration - AccelerationDuration ? DecelerationAlpha : AccelerationAlpha);

	// de-normalize the curve value so it returns a useable value rather than a point between 0 and 1
	//denormalized_d = normalized_d * (max_d - min_d) + min_d
	float NewSpeed = NormalizedNewSpeed * SpinSpeed;

	// finish spinning when the duration is complete, otherwise update fan's rotation
	FRotator NewRotation = FRotator(0, 0, clockwise ? -NewSpeed : NewSpeed);

	FQuat QuatRotation = FQuat(NewRotation);

	Mesh->AddLocalRotation(QuatRotation, false, 0);
}

void AThreeWaySwitch::StopSpinning(bool clockwise)
{
	GetWorld()->GetTimerManager().ClearTimer(SpinHandler);

	// update the switch's state and allow it to be interacted with again
	LastState = CurrentInteractablePips;
	AllowInteraction();
	UpdateColor();
}