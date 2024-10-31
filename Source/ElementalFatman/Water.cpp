// Fill out your copyright notice in the Description page of Project Settings.


#include "Water.h"

void AWater::Setup()
{
	if (OverrideMesh)
	{
		// replace mesh with variants here
	}

	UE_LOG(LogInteraction, Warning, TEXT("setting up water"));

	ObjectType = EObjectType::OT_Water;

	MaxInteractablePips = 2;
	CurrentInteractablePips = (int)StartAs;

	if (StartScale == FVector::Zero()) StartScale = Mesh->GetRelativeScale3D();
	if (EndScale == FVector::Zero()) EndScale = Mesh->GetRelativeScale3D();
	if (StartLocation == FVector::Zero()) StartLocation = Mesh->GetRelativeLocation();
	if (EndLocation == FVector::Zero()) EndLocation = Mesh->GetRelativeLocation();

	Super::Setup();
}

void AWater::InvokeSpecificMechanic()
{
	switch (CurrentInteractablePips)
	{
	case 0: // ice
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("created ice")));
		Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
		Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
		break;
	case 1: // water
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("created water")));
		Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
		Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

		// transform ice block to water scale/location
		if (!Melted) GetWorld()->GetTimerManager().SetTimer(MeltHandle, this, &AWater::Melt, GetWorld()->DeltaTimeSeconds, true);

		break;
	case 2: // steam
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("created steam, bye!!!")));
		this->Destroy();
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Water current pip value error!"));
		break;
	}
}

void AWater::Melt() 
{
	if (MeltAlpha >= 1) { StopMelting(); return; }

	MeltAlpha += GetWorld()->DeltaTimeSeconds / MeltDuration;

	FVector NewScale = WaterCurve->GetFloatValue(MeltAlpha) * (EndScale - StartScale) + StartScale;
	FVector NewLocation = WaterCurve->GetFloatValue(MeltAlpha) * (EndLocation - StartLocation) + StartLocation;

	Mesh->SetRelativeScale3D(NewScale);
	Mesh->SetRelativeLocation(NewLocation);
}

void AWater::StopMelting()
{
	GetWorld()->GetTimerManager().ClearTimer(MeltHandle);
	Melted = true; // don't need to run the melting animation more than once ever
}