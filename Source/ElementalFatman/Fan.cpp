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
	GetWorld()->GetTimerManager().ClearTimer(SpinHandler);

	// todo: once activated, fan moves until its mechanic is completed & then returns to idle, cannot be interacted with if its mechanic is already active
	switch (CurrentInteractablePips)
	{
	case 0: // clockwise
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("fan turning clockwise, %d"), CurrentInteractablePips));
		spinning = true;
		clockwise = true;
		GetWorld()->GetTimerManager().SetTimer(SpinHandler, this, &AFan::StopSpinning, SpinDuration);
		PowerStateChangedDelegate.Broadcast(this, CurrentInteractablePips);

		// prevent further interactions until fan is finished spinning
		MaxInteractablePips = 0;
		CurrentInteractablePips = 0;
		break;
	case 1: // idle
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("idling, can be heated/cooled now")));

		break;
	case 2: // anticlockwise
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("fan turning anticlockwise, %d"), CurrentInteractablePips));
		spinning = true;
		clockwise = false;
		GetWorld()->GetTimerManager().SetTimer(SpinHandler, this, &AFan::StopSpinning, SpinDuration);
		PowerStateChangedDelegate.Broadcast(this, CurrentInteractablePips);

		// prevent further interactions until fan is finished spinning
		MaxInteractablePips = 0;
		CurrentInteractablePips = 0;
		break;
	default: 
		UE_LOG(LogTemp, Error, TEXT("Fan current pip value error!"));
		break;
	}


}

void AFan::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (spinning)
	{
		FRotator NewRotation = FRotator(0, 0, clockwise ? SpinSpeed : -SpinSpeed);

		FQuat QuatRotation = FQuat(NewRotation);

		Mesh->AddLocalRotation(QuatRotation, false, 0);
	}
}

void AFan::StopSpinning() 
{
	// set back to idling
	spinning = false;
	GetWorld()->GetTimerManager().ClearTimer(SpinHandler);
	MaxInteractablePips = 2;
	CurrentInteractablePips = 1;
	PowerStateChangedDelegate.Broadcast(this, CurrentInteractablePips);
}