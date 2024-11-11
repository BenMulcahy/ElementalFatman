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
	OT_Mechanism UMETA(DisplayName = "Moving Mechanism"),
	OT_Pressure UMETA(DisplayName = "Pressure Plate"),
	OT_Barrel UMETA(DisplayName = "Expandable Barrel"),
	OT_Goo UMETA(DisplayName = "Goo"),
	OT_Box UMETA(DisplayName = "Movable Box"),
	OT_Torch UMETA(DisplayName = "Torch"),
	OT_Lamp UMETA(DisplayName = "Lamp"),
	OT_Grate UMETA(DisplayName = "Metal Grate"),
	OT_Clock UMETA(DisplayName = "Clock"),
	OT_Switch UMETA(DisplayName = "Three-Way Switch"),
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
	bool GetCanInteract() const { return CanInteract; }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	float WaitSeconds = 0;
	bool HasWaited = false;

	UPROPERTY(EditAnywhere, Category = Internal)
	UMeshComponent* Mesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Internal)
	UBoxComponent* BoxCollider = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Internal)
	UWidgetComponent* UIWidget = nullptr;

	UPROPERTY(EditAnywhere, Category = Internal)
	bool UiVisible = true;

	UPROPERTY(VisibleAnywhere, Category = Internal)
	UMaterialInstanceDynamic* DynamicMat = nullptr;


	UPROPERTY(BlueprintReadOnly, Category = Internal)
	EObjectType ObjectType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Internal)
	int32 CurrentInteractablePips = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Internal)
	int32 MaxInteractablePips = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Internal)
	int32 PipsPerInteract = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Internal)
	bool OverrideMesh = false;

	void UpdateColor();

	void UpdateUI();

	// override to prevent player from heating/cooling individual objects in specific use cases
	bool CanInteract = true;
	void PreventInteraction() { CanInteract = false; }
	void AllowInteraction() { CanInteract = true; CanHeat = true; CanCool = true; }

	bool CanHeat = true;
	void PreventHeat() { CanHeat = false; }

	bool CanCool = true;
	void PreventCool() { CanCool = false; }

};
