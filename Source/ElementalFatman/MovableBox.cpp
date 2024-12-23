// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableBox.h"

void AMovableBox::Setup()
{
	if (OverrideMesh)
	{
		// replace mesh with variants here
	}

	UE_LOG(LogInteraction, Warning, TEXT("setting up movable box"));

	ObjectType = EObjectType::OT_Movable_Box;

	MaxInteractablePips = 1;
	CurrentInteractablePips = StartFrozen ? 0 : 1;

	Super::Setup();
	InvokeSpecificMechanic();
}

void AMovableBox::Tick(float DeltaSeconds) 
{
	Super::Tick(DeltaSeconds);
	if (GetVelocity().Size() >= GrateBreakSpeed) { if (!Tags.Contains("Fast")) Tags.Add("Fast"); }
	else { if (Tags.Contains("Fast")) Tags.Remove("Fast"); }
}

void AMovableBox::InvokeSpecificMechanic()
{
	switch (CurrentInteractablePips)
	{
	case 0: // cooled/movable
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("released box")));
		Mesh->SetPhysMaterialOverride(SlipperyMat);
		Mesh->SetMassOverrideInKg(NAME_None, FrozenMass, true);
		break;
	case 1: // heated, locked in place
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("locked box in place")));
		Mesh->SetPhysMaterialOverride(RoughMat);
		Mesh->SetMassOverrideInKg(NAME_None, UnfrozenMass, true);
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Movable box current pip value error!"));
		break;
	}
}
