// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerSupply.h"
#include "Fan.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTALFATMAN_API AFan : public APowerSupply
{
	GENERATED_BODY()

protected:

	void Setup();
	void InvokeSpecificMechanic();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mechanics)
	int32 RotationSpeed = 3;

	bool clockwise = true;

};
