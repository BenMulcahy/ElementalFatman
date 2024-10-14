// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerSupply.h"
#include "MovingMechanism.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTALFATMAN_API AMovingMechanism : public APowerSupply
{
	GENERATED_BODY()

protected:

	void Setup();
	void InvokeSpecificMechanic();
};
