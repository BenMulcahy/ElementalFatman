// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerSupply.h"
#include "PowerReceiver.h"
#include "PowerSystem.generated.h"

UENUM()
enum class EPowerState : int8 
{
	Generator_Off = 0,
	Generator_On = 1,
	Fan_Off = 1,
	Fan_Clockwise = 0,
	Fan_Anticlockwise = 2
};

UCLASS()
class ELEMENTALFATMAN_API APowerSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdatePowerState(APowerSupply* UpdatedPowerSupply, int32 NewPowerState);

	bool IsPowerSupplied();

	bool AreEntriesValid();

	// Reference every object in the scene that will provide power to this interaction (e.g. generators, fans)
	UPROPERTY(EditAnywhere)
	TArray<APowerSupply*> PowerSuppliers;

	// In the same order as the power suppliers, indicate what state each one should be in to supply power to this system
	UPROPERTY(EditAnywhere)
	TArray<EPowerState> RequiredPowerStates;

	// Reference every object in the scene that will receive power from this interaction (e.g. doors)
	UPROPERTY(EditAnywhere)
	TArray<APowerReceiver*> PowerReceivers;

	UPROPERTY(VisibleAnywhere)
	TArray<int32> CurrentPowerStates;
};
