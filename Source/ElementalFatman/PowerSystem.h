// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerSupply.h"
#include "PowerSystem.generated.h"

UENUM()
enum class EPowerState : uint8
{
	PS_Generator_Off = 0,
	PS_Generator_On = 1,
	PS_Fan_Off = 1,
	PS_Fan_Clockwise = 0,
	PS_Fan_Anticlockwise = 2,
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

	void UpdatePowerState(int32 NewState, int32 ChildNum);

	bool CheckPowerStates();

	// Reference every object in the scene that will provide power to this interaction (e.g. generators, fans)
	UPROPERTY(EditAnywhere)
	TArray<APowerSupply*> PowerSuppliers;

	// In the same order as the Power Suppliers, list each object's required power state (0 = coldest, 1 = 
	UPROPERTY(EditAnywhere)
	TArray<EPowerState> RequiredPowerStates;

	TArray<int8> RequiredCurrentPips;

	// Reference every object in the scene that will receive power from this interaction (e.g. doors)
	UPROPERTY(EditAnywhere)
	TArray<AActor*> PowerReceivers;

	TArray<int32> CurrentPowerStates;
};
