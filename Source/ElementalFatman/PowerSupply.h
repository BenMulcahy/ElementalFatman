// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeatInteractable.h"
#include "PowerSupply.generated.h"

UCLASS()
class ELEMENTALFATMAN_API APowerSupply : public AHeatInteractable
{
	GENERATED_BODY()

protected:
	int RequiredPowerState;
};
