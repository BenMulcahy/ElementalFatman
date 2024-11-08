// Fill out your copyright notice in the Description page of Project Settings.


#include "Fan.h"

void AFan::Setup()
{
	if (!OverrideMesh)
	{
		// construct mesh here
	}

	ObjectType = EObjectType::OT_Fan;

	UE_LOG(LogInteraction, Warning, TEXT("setting up fan"));

	MaxInteractablePips = 1;
	CurrentInteractablePips = (int)StartState;

	// prevent single direction fans from being spun in the disallowed direction
	if (FanType == EFanLimitations::SingleDirectionClockwise) PreventHeat();
	else if (FanType == EFanLimitations::SingleDirectionAnticlockwise) PreventCool();

	Super::Setup();
}


void AFan::InvokeSpecificMechanic()
{
	FTimerDelegate SpinDelegate;
	// reset the lerp alphas to their original values
	AccelerationAlpha = 0;
	DecelerationAlpha = 1;

	GetWorld()->GetTimerManager().ClearTimer(SpinHandler);

	switch (CurrentInteractablePips)
	{
	case 0: // clockwise
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("fan turning clockwise, %d pips"), CurrentInteractablePips));
		SpinDelegate.BindUFunction(this, "Spin", true);
		GetWorld()->GetTimerManager().SetTimer(SpinHandler, SpinDelegate, GetWorld()->DeltaTimeSeconds, true);
		PowerStateChangedDelegate.Broadcast(this, CurrentInteractablePips);

		// prevent further interactions until fan is finished spinning
		PreventInteraction();
		break;
	case 1: // anticlockwise
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("fan turning anticlockwise, %d pips"), CurrentInteractablePips));
		SpinDelegate.BindUFunction(this, "Spin", false);
		GetWorld()->GetTimerManager().SetTimer(SpinHandler, SpinDelegate, GetWorld()->DeltaTimeSeconds, true);
		PowerStateChangedDelegate.Broadcast(this, CurrentInteractablePips);

		// prevent further interactions until fan is finished spinning
		PreventInteraction();
		break;
	default: 
		UE_LOG(LogTemp, Error, TEXT("Fan current pip value error!"));
		break;
	}
}

void AFan::Spin(bool clockwise)
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

void AFan::StopSpinning(bool clockwise) 
{
	// set back to idling
	GetWorld()->GetTimerManager().ClearTimer(SpinHandler);
	
	// reset its pips so it can now be interacted with again
	AllowInteraction();
	UpdateColor();
	
	// now spin in reverse/unpower the power system if it's a timed fan
	if (Timed) 
	{
		if (!TimedOut) // toggle to ensure the fan only spins in reverse once after each user input
		{
			CurrentInteractablePips = clockwise ? 1 : 0;
			InvokeSpecificMechanic();
			UpdateColor();
			TimedOut = true;
		}
		else TimedOut = false;
	}

	// apply limitations to single direction and normal fans
	if (FanType == EFanLimitations::SingleDirectionClockwise) PreventHeat();
	else if (FanType == EFanLimitations::SingleDirectionAnticlockwise) PreventCool();
	else if (FanType == EFanLimitations::Normal) clockwise ? PreventCool() : PreventHeat();
}