// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeatInteractable.generated.h"

UCLASS()
class ELEMENTALFATMAN_API AHeatInteractable : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int8 PipState = 0;

	UPROPERTY(EditAnywhere)
	bool InteractedWith;
	
public:	
	// Sets default values for this actor's properties
	AHeatInteractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
