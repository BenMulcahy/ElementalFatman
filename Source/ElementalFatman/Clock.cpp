// Fill out your copyright notice in the Description page of Project Settings.


#include "Clock.h"

void AClock::Setup()
{
	PreventInteraction();

	if (OverrideMesh)
	{
		// replace mesh with variants here
	}

	UE_LOG(LogInteraction, Warning, TEXT("setting up clock"));

	ObjectType = EObjectType::OT_Clock;

	MaxInteractablePips = 0;
	CurrentInteractablePips = 0;

	// add all child clock rings to an array for referencing
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (AActor* Attached : AttachedActors)
	{

		if (Cast<AClockRing>(Attached)) 
		{ 
			Rings.Add(Cast<AClockRing>(Attached)); 
			RingStates.Add(false);
		}
	}

	// subscribe to each ring's delegate that gets called when its rotation changes
	for (int i = 0; i < Rings.Num(); i++) 
	{ 
		Rings[i]->RotationChangedDelegate.AddUniqueDynamic(this, &AClock::UpdateRotation); 
	}

	// the clock starts incomplete/unpowered
	PowerStateChangedDelegate.Broadcast(this, 0);

	Super::Setup();

}

void AClock::UpdateRotation(AClockRing* ChangedRing, FRotator NewRotation) 
{
	int CorrectRings = 0;

	for (int i = 0; i < Rings.Num(); i++) // iterate through all the rings that are a child of this clock
	{
		// find and update the correct ring's state (if its rotation % 360 == 0, it's in the correct alignment)
		if (Rings[i] == ChangedRing) 
		{
			int Remainder = FMath::Abs(FMath::Modulo((int)NewRotation.Yaw, 360));
			RingStates[i] = Remainder == 0 ? true : false;
		}
		if (RingStates[i]) CorrectRings++; // count the number of correct rings (ringstates and rings arrays will always be the same length)
	}

	if (CorrectRings == RingStates.Num()) PowerStateChangedDelegate.Broadcast(this, 1); // if all rings are correct, power up this clock
	else PowerStateChangedDelegate.Broadcast(this, 0); // if any rings are incorrect, unpower this clock
}