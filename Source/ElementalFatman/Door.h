// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class ELEMENTALFATMAN_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

	void Open();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool opening = false;

	FTimerHandle OpenHandler;

	void StartOpening();

	void StopOpening();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OpenAmount = 5;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
