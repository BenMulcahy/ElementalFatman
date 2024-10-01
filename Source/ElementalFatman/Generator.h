// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerSupply.h"
#include "Generator.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTALFATMAN_API AGenerator : public APowerSupply
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
