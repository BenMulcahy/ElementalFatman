// Fill out your copyright notice in the Description page of Project Settings.


#include "Water.h"

void AWater::Setup()
{
	Super::Setup();

	if (OverrideMesh)
	{
		// replace mesh with variants here
	}

	UE_LOG(LogInteraction, Warning, TEXT("setting up water"));

	ObjectType = EObjectType::OT_Water;

	MaxInteractablePips = 2;
	CurrentInteractablePips = 1;

	InvokeSpecificMechanic(); // set starting state
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