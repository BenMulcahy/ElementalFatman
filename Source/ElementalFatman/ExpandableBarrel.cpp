// Fill out your copyright notice in the Description page of Project Settings.


#include "ExpandableBarrel.h"

void AExpandableBarrel::Setup()
{
	Super::Setup();

	if (OverrideMesh)
	{
		// replace mesh with variants here
	}

	UE_LOG(LogInteraction, Warning, TEXT("setting up water"));

	ObjectType = EObjectType::OT_Barrel;

	MaxInteractablePips = 1;
	CurrentInteractablePips = 0;

	InvokeSpecificMechanic(); // set starting state
}

void AExpandableBarrel::InvokeSpecificMechanic()
{
	switch (CurrentInteractablePips)
	{
	case 0: // unexpanded
		break;
	case 1: // expanded
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("expanded barrel")));
		// TODO: replace with animation
		GetWorld()->GetTimerManager().SetTimer(ExpandHandler, this, &AExpandableBarrel::Expand, GetWorld()->DeltaTimeSeconds, true);
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Barrel current pip value error!"));
		break;
	}
}

void AExpandableBarrel::Expand() 
{
	if (Mesh->GetRelativeScale3D().X >= ExpandSize) 
	{
		GetWorld()->GetTimerManager().ClearTimer(ExpandHandler);
		Tags.Add("Mantle");
		PreventInteraction();
		return;
	}
	float newScale = Mesh->GetRelativeScale3D().X + GetWorld()->DeltaTimeSeconds;
	Mesh->SetRelativeScale3D(FVector::One() * newScale);
}

