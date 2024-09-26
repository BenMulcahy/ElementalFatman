// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeatInteractable.h"
#include "Generator.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTALFATMAN_API AGenerator : public AHeatInteractable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool SwitchedOn;

protected:

	void Setup();
	void InvokeSpecificMechanic();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mechanics)
	AActor* Door;


};
