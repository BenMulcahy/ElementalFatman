// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerSupply.h"
#include "Fan.generated.h"

UCLASS()
class ELEMENTALFATMAN_API AFan : public APowerSupply
{
	GENERATED_BODY()

protected:

	void Setup();
	void InvokeSpecificMechanic();

	virtual void Tick(float DeltaTime) override;

	// Does this fan turn off after its spin duration, shutting off power?
	UPROPERTY(EditAnywhere, Category = Mechanics)
	bool Timed;

	UFUNCTION()
	void StopSpinning();
	FTimerHandle SpinHandler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mechanics)
	int32 SpinSpeed = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mechanics)
	float SpinDuration = 2.f;

	bool spinning = false;
	bool clockwise = true;

};
