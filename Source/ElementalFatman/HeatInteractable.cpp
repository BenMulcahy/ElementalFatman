#include "HeatInteractable.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "HeatInteractable.h"


// Sets default values
AHeatInteractable::AHeatInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHeatInteractable::BeginPlay()
{
	Super::BeginPlay();
	Setup();
	SetupInstancedMaterial();
	UpdateColor();
}

void AHeatInteractable::Setup() 
{
	PipsPerInteract = 1;

	switch (ObjectType)
	{
	case EObjectType::OT_HeatSource:
		MaxInteractablePips = 1;
		CurrentInteractablePips = 1;
		break;
	case EObjectType::OT_Water:
		MaxInteractablePips = 2;
		CurrentInteractablePips = 1;
		break;
	case EObjectType::OT_Barricade:
		MaxInteractablePips = 1;
		CurrentInteractablePips = 0;
		break;
	case EObjectType::OT_Generator:
		MaxInteractablePips = 1;
		CurrentInteractablePips = 0;
		break;
	case EObjectType::OT_Fan:
		MaxInteractablePips = 1;
		CurrentInteractablePips = 0;
		break;
	case EObjectType::OT_Lava:
		MaxInteractablePips = 1;
		CurrentInteractablePips = 1;
	default:
		break;
	}
}

void AHeatInteractable::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	if (ObjectType == EObjectType::OT_Fan) 
	{
		FRotator NewRotation = FRotator(0, 0, clockwise ? 3 : -3);

		FQuat QuatRotation = FQuat(NewRotation);

		AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);	
	}
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

int32 AHeatInteractable::ValidateInteraction(bool heating, int32 currentPlayerPips, int32 maxPlayerPips) 
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
				// decrease player pips by pipsperinteract
				playerNewPip = -PipsPerInteract;
			}
			else // player has insufficient pips
			{
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
				// increase player pips by pipsperinteract
				playerNewPip = PipsPerInteract;
			}
			else // player has too many pips
			{
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
	
	UpdateInteractable(playerNewPip);
	return playerNewPip;
}

void AHeatInteractable::UpdateInteractable(int32 interactionType) 
{
	if (interactionType == 0) return;
	
	if (interactionType < 0) // heating
	{
		// increase object pips by pipsperinteract (clamp)
		CurrentInteractablePips = FMath::Clamp(CurrentInteractablePips + PipsPerInteract, 0, MaxInteractablePips);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("heating, object pips: %d"), CurrentInteractablePips));
	}

	else // cooling
	{
		// decrease object pips by pipsperinteract (clamp)
		CurrentInteractablePips = FMath::Clamp(CurrentInteractablePips - PipsPerInteract, 0, MaxInteractablePips);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, FString::Printf(TEXT("cooling, object pips: %d"), CurrentInteractablePips));
	}


	// any other changes that happen to all interactables after successful interaction
	// e.g. update interactable's ui here

	switch (ObjectType)
	{
	case EObjectType::OT_HeatSource:
		break;
	case EObjectType::OT_Water:
		UpdateWater(CurrentInteractablePips);
		break;
	case EObjectType::OT_Barricade:
		DestroyBarricade(CurrentInteractablePips);
		break;
	case EObjectType::OT_Generator:
		SwitchGenerator(CurrentInteractablePips);
		break;
	case EObjectType::OT_Fan:
		RotateFan(CurrentInteractablePips);
		break;
	case EObjectType::OT_Lava:
		SolidifyLava(CurrentInteractablePips);
		break;
	default:
		break;
	}

	UE_LOG(LogInteraction, Warning, TEXT("object pips: %d"), CurrentInteractablePips);
	UpdateColor();
}

void AHeatInteractable::DestroyBarricade(int32 interactablePips)
{
	if (interactablePips < 1) return; // barricades do nothing when cooled

	// set timer, play animation
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("barricade destroyed")));
	this->Destroy();
}

void AHeatInteractable::SwitchGenerator(int32 interactablePips)
{
	if (interactablePips < 1) 
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("generator turned off")));
	}

	else 
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("generator turned on")));
		if (Door) Cast<ADoor>(Door)->Open();
	}
}

void AHeatInteractable::RotateFan(int32 interactablePips)
{
	if (interactablePips < 1) 
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("fan turning anticlockwise")));
		clockwise = false;
	}
	else 
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("fan turning clockwise")));
		clockwise = true;
	}
}

void AHeatInteractable::UpdateWater(int32 interactablePips) 
{
	if (interactablePips < 1)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("created ice")));
		Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	}
	else if (interactablePips > 1) 
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

void AHeatInteractable::SolidifyLava(int32 interactablePips) 
{
	if (interactablePips > 0) return;

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("solidified lava")));
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

}
