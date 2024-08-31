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

	UMeshComponent* Mesh;
	UMaterialInstanceDynamic* DynamicMat;
	void ChangeColor(FColor color);

	bool GetInteractedState() const { return InteractedWith; }

	void SetInteractedState(bool interactedWith) { this->InteractedWith = interactedWith; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	int8 PipState = 0;

	UPROPERTY(EditAnywhere)
	bool InteractedWith;

	void SetupInstancedMaterial();

//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;

};
