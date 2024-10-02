// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerReceiver.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTALFATMAN_API APowerReceiver : public AActor
{
	GENERATED_BODY()

public:
	bool GetIsPowered() const { return IsPowered; }
	void Power(bool ReceivingPower);

protected:
	bool IsPowered = false;

	virtual void Activate();
	virtual void Deactivate();
};
