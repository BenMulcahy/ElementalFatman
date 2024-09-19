// Fill out your copyright notice in the Description page of Project Settings.


#include "Lava.h"

ALava::ALava() 
{

}

void ALava::Setup()
{
	if (!OverrideMesh)
	{
		// construct mesh here
	}

	ObjectType = EObjectType::OT_Barricade;

	UE_LOG(LogInteraction, Warning, TEXT("setting up lava"));

	MaxInteractablePips = 1;
	CurrentInteractablePips = 1;
}


void ALava::InvokeSpecificMechanic(int32 interactionType)
{
	if (interactionType > 0) return;

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("solidified lava")));
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}