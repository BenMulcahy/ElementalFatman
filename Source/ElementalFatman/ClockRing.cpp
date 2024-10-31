// Fill out your copyright notice in the Description page of Project Settings.


#include "ClockRing.h"

void AClockRing::BeginPlay()
{
	Super::BeginPlay();

	// set the scale of this ring based on how far out from the center it is
	SetActorRelativeScale3D(GetActorRelativeScale3D() * 1 + (0.33f * RingNumber));
	// set the location of this ring based on how far out from the center it is
	SetActorRelativeLocation(FVector(0, 0, -100 * RingNumber));
	// set the starting angle of this ring
	LastRotation = FRotator(0, StartAngle, 0);
	SetActorRelativeRotation(LastRotation);

	// subscribe to each fan's delegate, which broadcasts whenever the fan is heated or cooled and returns its pip value
	for (int i = 0; i < Fans.Num(); i++)
	{
		Fans[i].Fan->PowerStateChangedDelegate.AddUniqueDynamic(this, &AClockRing::ChangeRotation);
	}
}

void AClockRing::ChangeRotation(APowerSupply* SpinningFan, int32 SpinDirection)
{
	FTimerDelegate TurnDelegate;
	TurnAlpha = 0;

	for (int i = 0; i < Fans.Num(); i++)
	{
		if (Fans[i].Fan == SpinningFan) // check which fan is spinning for instances where multiple fans affect 1 ring
		{
			float NewYaw = LastRotation.Yaw + (SpinDirection == 0 ? -Fans[i].TurnAngle : Fans[i].TurnAngle);
			TurnDelegate.BindUFunction(this, "Turn", NewYaw);
			GetWorld()->GetTimerManager().SetTimer(TurnHandler, TurnDelegate, GetWorld()->DeltaTimeSeconds, true);
		}
	}
}

void AClockRing::Turn(float TargetYaw) 
{
	// finish spinning when the total spin duration is over
	if (TurnAlpha >= 1) 
	{ 
		// ensure the actual end value is correct
		LastRotation.Yaw = TargetYaw;
		SetActorRelativeRotation(LastRotation);
		RotationChangedDelegate.Broadcast(this, LastRotation);

		StopTurning(); 
		return; 
	}

	TurnAlpha += GetWorld()->GetTime().GetDeltaWorldTimeSeconds() * TurnSpeed;

	float NormalizedYaw = ClockCurve->GetFloatValue(TurnAlpha);

	float NewYaw = NormalizedYaw * (TargetYaw - LastRotation.Yaw) + LastRotation.Yaw;

	LastRotation.Yaw = NewYaw;

	SetActorRelativeRotation(LastRotation);
}

void AClockRing::StopTurning() 
{
	GetWorld()->GetTimerManager().ClearTimer(TurnHandler);
}