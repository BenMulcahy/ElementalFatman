// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeatInteractable.h"
#include "PowerSupply.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPowerStateChanged, APowerSupply*, UpdatedPowerSupply, int32, NewPipState);

UCLASS()
class ELEMENTALFATMAN_API APowerSupply : public AHeatInteractable
{
	GENERATED_BODY()

public:
	FOnPowerStateChanged PowerStateChangedDelegate;

};
