// Fill out your copyright notice in the Description page of Project Settings.


#include "Lava.h"

ALava::ALava() 
{

}

void ALava::Setup()
{
	Super::Setup();

	if (!OverrideMesh)
	{
		// construct mesh here
	}

	ObjectType = EObjectType::OT_Lava;

	UE_LOG(LogInteraction, Warning, TEXT("setting up lava"));

	MaxInteractablePips = 0;
	CurrentInteractablePips = 1;

	// set initial lava collision state
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}


void ALava::InvokeSpecificMechanic()
{
	switch (CurrentInteractablePips)
	{
	case 0:
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("solidified lava")));
		Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	case 1:
		// do nothing atm, can you re-heat lava?
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Lava current pip value error!"));
		break;
	}

}