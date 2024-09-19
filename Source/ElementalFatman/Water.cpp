// Fill out your copyright notice in the Description page of Project Settings.


#include "Water.h"

void AWater::Setup()
{
	if (!OverrideMesh)
	{
		// construct mesh here
	}

	ObjectType = EObjectType::OT_Barricade;

	UE_LOG(LogInteraction, Warning, TEXT("setting up water"));

	MaxInteractablePips = 2;
	CurrentInteractablePips = 1;
}


void AWater::InvokeSpecificMechanic(int32 interactionType)
{
	if (interactionType < 1)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("created ice")));
		Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	}
	else if (interactionType > 1)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("created steam, bye!!!")));
		this->Destroy();
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("created water")));
		Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}
}