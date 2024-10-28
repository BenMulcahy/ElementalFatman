// Fill out your copyright notice in the Description page of Project Settings.


#include "Goo.h"

void AGoo::Setup()
{
	if (!OverrideMesh)
	{
		// construct mesh here
	}

	ObjectType = EObjectType::OT_Goo;

	UE_LOG(LogInteraction, Warning, TEXT("setting up goo"));

	MaxInteractablePips = 1;
	CurrentInteractablePips = 1;

	StartSize = Mesh->GetRelativeScale3D();

	Super::Setup();
}


void AGoo::InvokeSpecificMechanic()
{
	switch (CurrentInteractablePips)
	{
	case 0: // shrunk goo
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("shrunk goo")));
		GetWorld()->GetTimerManager().SetTimer(ShrinkHandler, this, &AGoo::Shrink, GetWorld()->GetTime().GetDeltaWorldTimeSeconds(), true);
		break;
	case 1:
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Goo current pip value error!"));
		break;
	}
}

void AGoo::Shrink() 
{
	// increase lerp alpha each frame so animation plays over the shrink duration
	ShrinkAlpha += GetWorld()->GetTime().GetDeltaWorldTimeSeconds() / ShrinkDuration;

	// get the normalised vector value on the curve at current alpha
	FVector NormalizedNewScale = ShrinkCurve->GetVectorValue(ShrinkAlpha);

	// de-normalize the curve value so it returns a useable value rather than a point between 0 and 1
	//denormalized_d = normalized_d * (max_d - min_d) + min_d
	FVector NewScale = NormalizedNewScale * (StartSize - (StartSize * ShrinkSize)) + (StartSize * ShrinkSize);

	// update the scale
	Mesh->SetRelativeScale3D(NewScale);

	if (ShrinkAlpha >= 1) StopShrinking();
}

void AGoo::StopShrinking() 
{
	GetWorld()->GetTimerManager().ClearTimer(ShrinkHandler);
	
	// go back to being able to interact with the box
	CurrentInteractablePips = 1;
	ShrinkAlpha = 0;
}