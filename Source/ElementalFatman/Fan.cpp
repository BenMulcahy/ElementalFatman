// Fill out your copyright notice in the Description page of Project Settings.


#include "Fan.h"

void AFan::Setup()
{
	Super::Setup();

	if (!OverrideMesh)
	{
		// construct mesh here
	}

	ObjectType = EObjectType::OT_Fan;

	UE_LOG(LogInteraction, Warning, TEXT("setting up fan"));

	MaxInteractablePips = 2;
	CurrentInteractablePips = 1;
}


void AFan::InvokeSpecificMechanic()
{
	FTimerDelegate SpinDelegate;
	// reset the lerp alphas to their original values
	AccelerationAlpha = 0;
	DecelerationAlpha = 1;

	GetWorld()->GetTimerManager().ClearTimer(SpinHandler);

	// todo: once activated, fan moves until its mechanic is completed & then returns to idle, cannot be interacted with if its mechanic is already active
	switch (CurrentInteractablePips)
	{
	case 0: // clockwise
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("fan turning clockwise, %d"), CurrentInteractablePips));
		SpinDelegate.BindUFunction(this, "Spin", true);
		GetWorld()->GetTimerManager().SetTimer(SpinHandler, SpinDelegate, GetWorld()->DeltaTimeSeconds, true);
		PowerStateChangedDelegate.Broadcast(this, CurrentInteractablePips);

		// prevent further interactions until fan is finished spinning
		PreventInteraction();
		CurrentInteractablePips = 0;
		break;
	case 1: // idle
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("idling, can be heated/cooled now")));

		break;
	case 2: // anticlockwise
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("fan turning anticlockwise, %d"), CurrentInteractablePips));
		SpinDelegate.BindUFunction(this, "Spin", false);
		GetWorld()->GetTimerManager().SetTimer(SpinHandler, SpinDelegate, GetWorld()->DeltaTimeSeconds, true);		
		PowerStateChangedDelegate.Broadcast(this, CurrentInteractablePips);

		// prevent further interactions until fan is finished spinning
		PreventInteraction();
		CurrentInteractablePips = 0;
		break;
	default: 
		UE_LOG(LogTemp, Error, TEXT("Fan current pip value error!"));
		break;
	}
}

void AFan::Spin(bool clockwise)
{
	// finish spinning when the total spin duration is over
	if (AccelerationAlpha >= SpinDuration) 
	{ 
		StopSpinning(); 
		return; 
	}

	else 
	{
		// start decelerating towards the end of the spin duration
		if (AccelerationAlpha >= SpinDuration - AccelerationDuration) DecelerationAlpha += GetWorld()->GetTime().GetDeltaWorldTimeSeconds() / -AccelerationDuration;

		// start accelerating and keep track of the time the fan has been spinning
		AccelerationAlpha += GetWorld()->GetTime().GetDeltaWorldTimeSeconds() / AccelerationDuration;
		
		// take a point from the acceleration curve based on either the acceleration or deceleration alpha
		float NormalizedNewSpeed = AccelerationCurve->GetFloatValue(AccelerationAlpha >= SpinDuration - AccelerationDuration ? DecelerationAlpha : AccelerationAlpha);

		// de-normalize the curve value so it returns a useable value rather than a point between 0 and 1
		//denormalized_d = normalized_d * (max_d - min_d) + min_d
		float NewSpeed = NormalizedNewSpeed * SpinSpeed;

		// finish spinning when the duration is complete, otherwise update fan's rotation
		FRotator NewRotation = FRotator(0, 0, clockwise ? NewSpeed : -NewSpeed);

		FQuat QuatRotation = FQuat(NewRotation);

		Mesh->AddLocalRotation(QuatRotation, false, 0);
	}
}

void AFan::StopSpinning() 
{
	// set back to idling
	GetWorld()->GetTimerManager().ClearTimer(SpinHandler);
	
	// reset its pips so it can now be interacted with again
	MaxInteractablePips = 2;
	CurrentInteractablePips = 1;
	
	// switch off the power it supplies if it's a timed fan
	if (Timed) PowerStateChangedDelegate.Broadcast(this, CurrentInteractablePips);
}