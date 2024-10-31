// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlate.h"

APressurePlate::APressurePlate() 
{
	// Mesh is the root component & therefore cannot be moved independently of the collider, so using a 2nd mesh component (sibling of the collider) for BPs of this script
	MovableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Movable Mesh"));
	MovableMesh->SetupAttachment(RootComponent);
}

void APressurePlate::Setup()
{
	PreventInteraction();

	if (OverrideMesh)
	{
		// replace mesh with variants here
	}

	UE_LOG(LogInteraction, Warning, TEXT("setting up pressure plate"));

	ObjectType = EObjectType::OT_Pressure;

	MaxInteractablePips = 0;
	CurrentInteractablePips = 0;

	StartPos = MovableMesh->GetRelativeLocation().Z;

	Super::Setup();

	PowerStateChangedDelegate.Broadcast(this, 0);
}

void APressurePlate::InvokeSpecificMechanic() { } // pressure plate isn't actually a heat interactable so no specific mechanic to invoke

void APressurePlate::Press() 
{
	SetPressed(true);

	GetWorld()->GetTimerManager().ClearTimer(PressHandle);
	FTimerDelegate PressDelegate;
	PressDelegate.BindUFunction(this, "Move", true); // bind function to delegate so it can take params
	PressAlpha = 0;
	GetWorld()->GetTimerManager().SetTimer(PressHandle, PressDelegate, GetWorld()->GetTime().GetDeltaWorldTimeSeconds(), true);
	PowerStateChangedDelegate.Broadcast(this, 1);
}

void APressurePlate::Reset() 
{
	SetPressed(false);

	GetWorld()->GetTimerManager().ClearTimer(PressHandle);
	FTimerDelegate PressDelegate;
	PressDelegate.BindUFunction(this, "Move", false); // bind function to delegate so it can take params
	PressAlpha = 1;
	GetWorld()->GetTimerManager().SetTimer(PressHandle, PressDelegate, GetWorld()->GetTime().GetDeltaWorldTimeSeconds(), true);
	PowerStateChangedDelegate.Broadcast(this, 0);
}

void APressurePlate::Move(bool pressed) 
{
	// increase or decrease lerp alpha each frame by delta time / duration, depending on whether plate is being pressed or released
	PressAlpha += pressed ? GetWorld()->GetTime().GetDeltaWorldTimeSeconds() / PressDuration : GetWorld()->GetTime().GetDeltaWorldTimeSeconds() / -PressDuration;

	// get the normalised vector value on the curve at current alpha
	float NormalizedNewPos = PressCurve->GetFloatValue(PressAlpha);

	// de-normalize the curve value so it returns a useable value rather than a point between 0 and 1
	//denormalized_d = normalized_d * (max_d - min_d) + min_d
	float NewPos = NormalizedNewPos * (PressedPos - StartPos) + StartPos;

	FVector MeshLocation = MovableMesh->GetRelativeLocation();
	MeshLocation.Z = NewPos;

	// update the position
	MovableMesh->SetRelativeLocation(MeshLocation);

	// clear the timer when the animation is finished
	if (pressed && PressAlpha >= 1) GetWorld()->GetTimerManager().ClearTimer(PressHandle);
	if (!pressed && PressAlpha <= 0) GetWorld()->GetTimerManager().ClearTimer(PressHandle);
}
