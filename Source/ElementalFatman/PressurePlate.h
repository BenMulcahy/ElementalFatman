// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PressurePlate.generated.h"

UCLASS()
class ELEMENTALFATMAN_API APressurePlate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APressurePlate();

	UFUNCTION(BlueprintCallable)
	void Press();

	UFUNCTION(BlueprintCallable)
	void Reset();

	UPROPERTY(EditAnywhere)
	AActor* DeleteThis = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	USceneComponent* Root = nullptr;

	UPROPERTY(EditAnywhere)
	UMeshComponent* Mesh = nullptr;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxCollider = nullptr;

	UPROPERTY(EditAnywhere)
	float PressedPosition;

	UPROPERTY(BlueprintReadWrite)
	float StartPos;
};
