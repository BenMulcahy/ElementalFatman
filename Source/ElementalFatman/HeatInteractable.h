// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElementalFatman.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "HeatInteractable.generated.h"

UENUM()
enum class EObjectType : uint8
{
	OT_HeatSource UMETA(DisplayName = "HeatSource"),
	OT_Water UMETA(DisplayName = "Water"),
	OT_Barricade UMETA(DisplayName = "Barricade"),
	OT_Generator UMETA(DisplayName = "Generator"),
	OT_Fan UMETA(DisplayName = "Fan"),
	OT_Lava UMETA(DisplayName = "Lava"),
};

UCLASS()
class ELEMENTALFATMAN_API AHeatInteractable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHeatInteractable();

	int32 ValidateInteraction(bool heating, int32 currentPlayerPips, int32 maxPlayerPips);

	void UpdateInteractable(int32 interactionType);

	// virtual allows a function of the same name to be automatically called on child classes -- need to look into this more
	virtual void Setup();

	virtual void InvokeSpecificMechanic();

	int32 GetCurrentInteractablePips()  const { return CurrentInteractablePips; }
	EObjectType GetObjectType() const { return ObjectType; }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	USceneComponent* Root = nullptr;

	UPROPERTY(EditAnywhere)
	UMeshComponent* Mesh = nullptr;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxCollider = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* UIWidget = nullptr;

	UMaterialInstanceDynamic* DynamicMat = nullptr;


	UPROPERTY(BlueprintReadOnly, Category = Mechanics)
	EObjectType ObjectType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mechanics)
	int32 CurrentInteractablePips = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mechanics)
	int32 MaxInteractablePips = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mechanics)
	int32 PipsPerInteract = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mechanics)
	bool OverrideMesh = false;

	void UpdateColor();

	void UpdateUI();


public:
};
