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
}

void AHeatInteractable::SetupInstancedMaterial()
{
	Mesh = CastChecked<UMeshComponent>(GetComponentByClass<UMeshComponent>());
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

int32 AHeatInteractable::AttemptInteraction(bool heating, int32 currentPlayerPips) 
{
	int32 playerNewPip = 0;

	if (heating && CurrentInteractablePips < MaxInteractablePips) 
	{
		if (currentPlayerPips >= PipsPerInteract) 
		{
			//TODO: Fix this -> doesnt work with pipsPerInteract properly
			CurrentInteractablePips++;
			UE_LOG(LogInteraction, Warning, TEXT("interactable new pipstate: %d"), CurrentInteractablePips);
			playerNewPip = CurrentInteractablePips < PipsPerInteract ? -CurrentInteractablePips : -PipsPerInteract;
		}
		else 
		{
			// ui for not enough pips
			UE_LOG(LogInteraction, Warning, TEXT("you don't have enough pips to heat this object!"), CurrentInteractablePips);
		}
	}
	else if (!heating && CurrentInteractablePips > 0)
	{
		CurrentInteractablePips--;
		UE_LOG(LogInteraction, Warning, TEXT("interactable new pipstate: %d"), CurrentInteractablePips);
		playerNewPip = CurrentInteractablePips < PipsPerInteract ? CurrentInteractablePips : PipsPerInteract;
	}
	else 
	{ 
		UE_LOG(LogInteraction, Warning, TEXT("can't get any hotter or maybe colder!!!!")); 
	}

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Magenta, FString::Printf(TEXT("%d"), CurrentInteractablePips));
	
	UpdateColor();
	return playerNewPip;
}
