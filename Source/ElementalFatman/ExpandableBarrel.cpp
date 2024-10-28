// Fill out your copyright notice in the Description page of Project Settings.


#include "ExpandableBarrel.h"

void AExpandableBarrel::Setup()
{
	if (OverrideMesh)
	{
		// replace mesh with variants here
	}

	UE_LOG(LogInteraction, Warning, TEXT("setting up water"));

	ObjectType = EObjectType::OT_Barrel;

	MaxInteractablePips = 1;
	CurrentInteractablePips = 0;

	StartSize = Mesh->GetRelativeScale3D().X;

	Super::Setup();
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
		PreventInteraction();
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Barrel current pip value error!"));
		break;
	}
}

void AExpandableBarrel::Expand() 
{
	// expandalpha increases from 0 to 1 over the expandduration variable
	ExpandAlpha += GetWorld()->GetTime().GetDeltaWorldTimeSeconds() / ExpandDuration;

	// take a point from the curve based on expandalpha
	float NormalizedNewScale = ExpandCurve->GetFloatValue(ExpandAlpha);

	// de-normalize the curve value so it returns a useable value rather than a point between 0 and 1
	//denormalized_d = normalized_d * (max_d - min_d) + min_d
	float NewScale = NormalizedNewScale * (ExpandSize - StartSize) + StartSize;


	// update scale
	Mesh->SetRelativeScale3D(FVector::One() * NewScale);

	if (ExpandAlpha >= 1)
	{
		GetWorld()->GetTimerManager().ClearTimer(ExpandHandler);
		Tags.Add("Mantle"); // box becomes mantleable when expanded
		return;
	}
}

