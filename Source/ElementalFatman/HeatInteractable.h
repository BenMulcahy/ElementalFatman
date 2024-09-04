// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeatInteractable.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mechanics)
	int32 CurrentInteractablePips = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mechanics)
	int32 MaxInteractablePips = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mechanics)
	int32 PipsPerInteract = 1;

	void SetupInstancedMaterial();

	virtual void UpdateInteractable(int32 interactionType);

	void UpdateColor();


//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;

};
