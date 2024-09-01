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
	void UpdateColor();

	int32 AttemptInteraction(bool heating);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InteractablePipState = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxInteractablePips = 3;

	void SetupInstancedMaterial();

//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;

};
