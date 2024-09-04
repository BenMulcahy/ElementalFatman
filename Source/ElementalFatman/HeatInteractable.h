// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElementalFatman.h"
#include "Door.h"
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

	UMeshComponent* Mesh = nullptr;
	UMaterialInstanceDynamic* DynamicMat = nullptr;

	virtual int32 ValidateInteraction(bool heating, int32 currentPlayerPips, int32 maxPlayerPips);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Setup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mechanics)
	EObjectType ObjectType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mechanics)
	int32 CurrentInteractablePips = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mechanics)
	int32 MaxInteractablePips = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mechanics)
	int32 PipsPerInteract = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mechanics)
	AActor* Door;

	bool clockwise = true;

	void SetupInstancedMaterial();

	virtual void UpdateInteractable(int32 interactionType);

	void DestroyBarricade(int32 interactablePips);

	void SwitchGenerator(int32 interactablePips);

	void RotateFan(int32 interactablePips);

	void UpdateWater(int32 interactablePips);

	void SolidifyLava(int32 interactablePips);

	FTimerHandle RotateTimerHandle;

	void UpdateColor();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
