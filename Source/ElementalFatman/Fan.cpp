// Fill out your copyright notice in the Description page of Project Settings.


#include "Fan.h"

void AFan::Setup()
{
	Super::Setup();

	if (!OverrideMesh)
	{
		// construct mesh here
	}

	ObjectType = EObjectType::OT_Fan;

	UE_LOG(LogInteraction, Warning, TEXT("setting up fan"));

	MaxInteractablePips = 1;
	CurrentInteractablePips = 0;
}


void AFan::InvokeSpecificMechanic()
{
	switch (CurrentInteractablePips)
	{
	case 0:
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("fan turning clockwise")));
		clockwise = true;		
		break;
	case 1:
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("fan turning anticlockwise")));
		clockwise = false;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Fan current pip value error!"));
		break;
	}
}

void AFan::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FRotator NewRotation = FRotator(0, 0, clockwise ? RotationSpeed : -RotationSpeed);

	FQuat QuatRotation = FQuat(NewRotation);

	Mesh->AddLocalRotation(QuatRotation, false, 0);

}