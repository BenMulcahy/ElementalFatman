// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerSupply.h"
#include "PressurePlate.generated.h"

UCLASS()
class ELEMENTALFATMAN_API APressurePlate : public APowerSupply
{
	GENERATED_BODY()
	
public:	

	UFUNCTION(BlueprintCallable)
	void Press();

	UFUNCTION(BlueprintCallable)
	void Reset();

protected:

	void Setup();
	void InvokeSpecificMechanic();

	UPROPERTY(EditAnywhere)
	float PressedPosition;

	UPROPERTY(BlueprintReadWrite)
	float StartPos;
};
