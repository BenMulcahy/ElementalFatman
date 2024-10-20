// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableBox.h"

void AMovableBox::Setup()
{
	Super::Setup();

	if (OverrideMesh)
	{
		// replace mesh with variants here
	}

	UE_LOG(LogInteraction, Warning, TEXT("setting up movable box"));

	ObjectType = EObjectType::OT_Box;

	MaxInteractablePips = 1;
	CurrentInteractablePips = 1;
}

void AMovableBox::InvokeSpecificMechanic()
{
	switch (CurrentInteractablePips)
	{
	case 0: // cooled/movable
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("released box")));
		Mesh->SetPhysMaterialOverride(SlipperyMat);
		break;
	case 1: // heated, locked in place
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("locked box in place")));
		Mesh->SetPhysMaterialOverride(RoughMat);
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Movable box current pip value error!"));
		break;
	}
}
