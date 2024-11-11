// Fill out your copyright notice in the Description page of Project Settings.


#include "ClockRing.h"

AClockRing::AClockRing() 
{
	PrimaryActorTick.bCanEverTick = true;
}

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

	// subscribe to each switch's delegate, which broadcasts whenever the switch is spun and its direction
	for (int i = 0; i < Switches.Num(); i++)
	{
		Switches[i].Switch->SwitchSpinningDelegate.AddUniqueDynamic(this, &AClockRing::ChangeRotation);
	}

	SetupPressurePlates();
}

void AClockRing::Tick(float DeltaSeconds) 
{
	Super::Tick(DeltaSeconds);

	if (ShowDebugLine)
	{
		for (int i = 0; i < Switches.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("peep"));
			DrawDebugLine(GetWorld(), Switches[i].Switch->GetActorLocation(), FindComponentByTag<UStaticMeshComponent>("Pointer")->GetComponentLocation(), Switches[i].SwitchDisconnected ? FColor::Red : FColor::Yellow, false);
			if (Switches[i].ControlledByPressurePlate) DrawDebugLine(GetWorld(), Switches[i].Switch->GetActorLocation(), Switches[i].PressurePlate->GetActorLocation(), Switches[i].SwitchDisconnected ? FColor::Red : FColor::Green, false);
		}
	}
}

void AClockRing::SetupPressurePlates() 
{	
	// for each switch with a pressure plate component, subscribe to the pressure plate's delegate, which broadcasts whenever the pressure plate is activated/deactivated
	for (int i = 0; i < Switches.Num(); i++)
	{
		if (Switches[i].ControlledByPressurePlate) Switches[i].PressurePlate->PowerStateChangedDelegate.AddUniqueDynamic(this, &AClockRing::DisconnectSwitch);
	}
}

void AClockRing::DisconnectSwitch(APowerSupply* UpdatedPressurePlate, int32 NewPipState)
{
	// find the switch whose pressure plate's state changed, disconnect or re-connect that switch to the clock ring depending on the updated pressure plate's pip state
	for (int i = 0; i < Switches.Num(); i++)
	{
		if (Switches[i].PressurePlate == UpdatedPressurePlate) 
		{
			Switches[i].SwitchDisconnected = NewPipState == 1 ? true : false;
		}
	}
}

void AClockRing::ChangeRotation(AThreeWaySwitch* ChangedSwitch, bool SpinDirection)
{
	FTimerDelegate TurnDelegate;
	TurnAlpha = 0;

	for (int i = 0; i < Switches.Num(); i++)
	{
		if (Switches[i].Switch == ChangedSwitch) // check which switch is spinning for instances where multiple switches affect 1 ring
		{
			if (Switches[i].SwitchDisconnected) return; // don't spin the ring if the switch has been disconnected
			float NewYaw = LastRotation.Yaw + (SpinDirection ? Switches[i].TurnAngle : -Switches[i].TurnAngle);
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