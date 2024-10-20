#include "HeatInteractable.h"
// Fill out your copyright notice in the Description page of Project Settings.


// Sets default values
AHeatInteractable::AHeatInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	// construct box collider
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	BoxCollider->SetupAttachment(Mesh);

	// construct ui widget (displays pip count)
	UIWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("UI Widget"));
	UIWidget->SetupAttachment(RootComponent);
	UIWidget->SetRelativeLocation(FVector(0, 0, 90));
	UIWidget->SetWidgetSpace(EWidgetSpace::Screen);
	UIWidget->SetDrawSize(FVector2D(50, 50));

	// when creating a new heat interactable bp, go to the bp editor and please:
	// assign a mesh (e.g. cube)
	// assign the heat interactable material
	// assign the pip display ui to the widget
	// copy the pip ui update function in bp
}

// Called when the game starts or when spawned
void AHeatInteractable::BeginPlay()
{
	Super::BeginPlay();

	Setup();
	UpdateColor();
	UpdateUI();
}

void AHeatInteractable::Setup() 
{
	PipsPerInteract = 1; // currently all interactables use/give one pip, could be changed in the future

	// set up temporary dynamic material so can change object's colour to indicate heat/cool state
	DynamicMat = UMaterialInstanceDynamic::Create(Mesh->GetMaterials()[0], this);
	Mesh->SetMaterial(0, DynamicMat);
}

void AHeatInteractable::UpdateColor()
{
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
	// validation failed
	if (interactionType == 0) return;
	
	if (interactionType < 0) // successfully heated
	{
		// increase object pips by pipsperinteract (clamp)
		CurrentInteractablePips = FMath::Clamp(CurrentInteractablePips + PipsPerInteract, 0, MaxInteractablePips);
	}

	else // successfully cooled
	{
		// decrease object pips by pipsperinteract (clamp)
		CurrentInteractablePips = FMath::Clamp(CurrentInteractablePips - PipsPerInteract, 0, MaxInteractablePips);
	}

	// any other changes that happen to all interactables after successful interaction go here
	UpdateColor();
	UpdateUI();

	// update unique interactables
	InvokeSpecificMechanic();
}

void AHeatInteractable::InvokeSpecificMechanic() {} // virtual function for child classes

void AHeatInteractable::UpdateUI() 
{
	UFunction* UIFunction = FindFunction(TEXT("UpdatePipUI"));
	ProcessEvent(UIFunction, nullptr);
}

void AHeatInteractable::PreventInteraction() 
{
	MaxInteractablePips = 0;
	CurrentInteractablePips = 0;
}
