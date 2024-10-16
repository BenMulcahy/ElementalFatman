// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlate.h"

void APressurePlate::Setup()
{
	Super::Setup();

	if (OverrideMesh)
	{
		// replace mesh with variants here
	}

	UE_LOG(LogInteraction, Warning, TEXT("setting up pressure plate"));

	ObjectType = EObjectType::OT_Pressure;

	BoxCollider->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	MaxInteractablePips = 0;
	CurrentInteractablePips = 0;

	StartPos = Mesh->GetRelativeLocation().Z;
}

void APressurePlate::InvokeSpecificMechanic() { } // pressure plate isn't actually a heat interactable so no specific mechanic to invoke

void APressurePlate::Press() 
{
	GetWorld()->GetTimerManager().ClearTimer(PressHandle);
	FTimerDelegate PressDelegate;
	PressDelegate.BindUFunction(this, "Move", true); // bind function to delegate so it can take params
	PressAlpha = 0;
	GetWorld()->GetTimerManager().SetTimer(PressHandle, PressDelegate, GetWorld()->GetTime().GetDeltaWorldTimeSeconds(), true);
	PowerStateChangedDelegate.Broadcast(this, 1);
}

void APressurePlate::Reset() 
{
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

	FVector MeshLocation = Mesh->GetRelativeLocation();
	MeshLocation.Z = NewPos;

	// update the position
	Mesh->SetRelativeLocation(MeshLocation);

	// clear the timer when the animation is finished
	if (pressed && PressAlpha >= 1) GetWorld()->GetTimerManager().ClearTimer(PressHandle);
	if (!pressed && PressAlpha <= 0) GetWorld()->GetTimerManager().ClearTimer(PressHandle);
}
