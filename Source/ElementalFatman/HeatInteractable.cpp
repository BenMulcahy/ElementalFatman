// Fill out your copyright notice in the Description page of Project Settings.


#include "HeatInteractable.h"
#include "ElementalFatman.h"

// Sets default values
AHeatInteractable::AHeatInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AHeatInteractable::BeginPlay()
{
	Super::BeginPlay();
	SetupInstancedMaterial();
	UpdateColor();
}

void AHeatInteractable::SetupInstancedMaterial()
{
	Mesh = Cast<UMeshComponent>(GetComponentByClass<UMeshComponent>());
	if (Mesh) { UE_LOG(LogTemp, Warning, TEXT("Found mesh: %s"), *Mesh->GetName()); }
	else UE_LOG(LogTemp, Warning, TEXT("No mesh found"));

	DynamicMat = UMaterialInstanceDynamic::Create(Mesh->GetMaterials()[0], this);
	if (!IsValid(DynamicMat)) { UE_LOG(LogTemp, Error, TEXT("Sorry! No dynamic material for you")) return;}
	Mesh->SetMaterial(0, DynamicMat);
}

void AHeatInteractable::UpdateColor()
{
	if (!IsValid(DynamicMat)) { UE_LOG(LogTemp, Error, TEXT("Woopsie! No Dynamic matieral here lmao")); return; }
	DynamicMat->SetVectorParameterValue("TestColour", CurrentInteractablePips == 0 ? FColor::Blue : CurrentInteractablePips == MaxInteractablePips ? FColor::Red : FColor::White);
}

int32 AHeatInteractable::AttemptInteraction(bool heating, int32 currentPlayerPips, int32 maxPlayerPips) 
{
	int32 playerNewPip = 0;

	// check heating
	if (heating)
	{
		// check object can be heated any further
		if (CurrentInteractablePips < MaxInteractablePips)
		{
			// check player has enough pips to heat object
			if (currentPlayerPips >= PipsPerInteract)
			{
				// increase object pips by pipsperinteract (clamp)
				CurrentInteractablePips = FMath::Clamp(CurrentInteractablePips + PipsPerInteract, 0, MaxInteractablePips);
				UE_LOG(LogInteraction, Warning, TEXT("object pips: %d"), CurrentInteractablePips);
				if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("heating, object pips: %d"), CurrentInteractablePips));
				
				// decrease player pips by pipsperinteract
				playerNewPip = -PipsPerInteract;
			}
			else // player has insufficient pips
			{
				// ui for not enough pips
				UE_LOG(LogInteraction, Warning, TEXT("you don't have enough pips to heat this object!"));
				if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Magenta, FString::Printf(TEXT("you don't have enough pips to heat this object!")));
			}
		}
		else // object is fully heated
		{
			UE_LOG(LogInteraction, Warning, TEXT("object fully heated!"));
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("object fully heated!")));
		}
	}

	// check cooling
	else
	{
		// check object can be cooled any further
		if (CurrentInteractablePips > 0)
		{
			// check player is able to gain pips from this
			if (currentPlayerPips + PipsPerInteract <= maxPlayerPips)
			{
				// decrease object pips by pipsperinteract (clamp)
				CurrentInteractablePips = FMath::Clamp(CurrentInteractablePips - PipsPerInteract, 0, MaxInteractablePips);
				UE_LOG(LogInteraction, Warning, TEXT("object pips: %d"), CurrentInteractablePips);
				if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, FString::Printf(TEXT("cooling, object pips: %d"), CurrentInteractablePips));

				// increase player pips by pipsperinteract
				playerNewPip = PipsPerInteract;
			}
			else // player has too many pips
			{
				// ui for too many pips
				UE_LOG(LogInteraction, Warning, TEXT("you can't gain any more pips!"));
				if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Magenta, FString::Printf(TEXT("you can't gain any more pips!")));
			}
		}
		else // object is fully cooled
		{
			UE_LOG(LogInteraction, Warning, TEXT("object fully cooled!"));
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, FString::Printf(TEXT("object fully cooled!")));
		}
	}
	
	UpdateColor();
	return playerNewPip;
}
