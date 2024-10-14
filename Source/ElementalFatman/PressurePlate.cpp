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

	MaxInteractablePips = 0;
	CurrentInteractablePips = 0;

	StartPos = Mesh->GetRelativeLocation().Z;
}

void APressurePlate::InvokeSpecificMechanic() { } // pressure plate isn't actually a heat interactable so no specific mechanic to invoke

void APressurePlate::Press() 
{
	Mesh->SetRelativeLocation(FVector(Mesh->GetRelativeLocation().X, Mesh->GetRelativeLocation().Y, PressedPosition));
	PowerStateChangedDelegate.Broadcast(this, 1);
}

void APressurePlate::Reset() 
{
	Mesh->SetRelativeLocation(FVector(Mesh->GetRelativeLocation().X, Mesh->GetRelativeLocation().Y, StartPos));
	PowerStateChangedDelegate.Broadcast(this, 0);
}
