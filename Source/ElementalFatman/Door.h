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
	void Close();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ClosedLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector OpenLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OpenAmount = 5;
	
	float Position = 0;
	bool IsOpen = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
