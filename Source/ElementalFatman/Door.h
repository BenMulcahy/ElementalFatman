// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerReceiver.h"
#include "Door.generated.h"

UCLASS()
class ELEMENTALFATMAN_API ADoor : public APowerReceiver
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

	UFUNCTION(BlueprintCallable)
	void Open();

	UFUNCTION(BlueprintCallable)
	void Close();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Activate();
	void Deactivate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ClosedLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector OpenLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration;
	
	float Position = 0;
	bool IsOpen = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
