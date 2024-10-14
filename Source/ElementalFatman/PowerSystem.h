// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeatInteractable.h"
#include "PowerSupply.h"
#include "PowerReceiver.h"
#include "Generator.h"
#include "Fan.h"
#include "PressurePlate.h"
#include "MovingMechanism.h"
#include "PowerSystem.generated.h"

UENUM()
enum class ESupplyType : int8
{
	ST_Default = 0,
	ST_Generator = 1,
	ST_Fan = 2,
	ST_Pressure = 3,
	ST_Moving = 4,
};

UENUM()
enum class EGeneratorState : int8
{
	Off = 0,
	On = 1,
};

UENUM()
enum class EFanState : int8
{
	Off = 1,
	Clockwise = 0,
	Anticlockwise = 2,
};

UENUM()
enum class EPressurePlateState : int8 
{
	Released = 0,
	Pressed = 1,
};

UENUM()
enum class EMovingMechanism : int8 
{
	On = 0,
	Off = 1,
};

UCLASS(EditInlineNew, DefaultToInstanced)
class ELEMENTALFATMAN_API UPowerSupplierInstance : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "New Power Supplier")
	APowerSupply* PowerSupply = nullptr;

	UPROPERTY()
	ESupplyType TypeOfSupply;

	UPROPERTY(EditAnywhere, Category = "New Power Supplier", meta=(EditCondition="TypeOfSupply == ESupplyType::ST_Generator", EditConditionHides))
	EGeneratorState GeneratorMustBe;

	UPROPERTY(EditAnywhere, Category = "New Power Supplier", meta = (EditCondition = "TypeOfSupply == ESupplyType::ST_Fan", EditConditionHides))
	EFanState FanMustBe;

	UPROPERTY(EditAnywhere, Category = "New Power Supplier", meta = (EditCondition = "TypeOfSupply == ESupplyType::ST_Pressure", EditConditionHides))
	EPressurePlateState PressurePlateMustBe;

	UPROPERTY(EditAnywhere, Category = "New Power Supplier", meta = (EditCondition = "TypeOfSupply == ESupplyType::ST_Moving", EditConditionHides))
	EMovingMechanism MovingMechanismMustBe;
	
protected:

	UPowerSupplierInstance();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

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

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void UpdatePowerState(APowerSupply* UpdatedPowerSupply, int32 NewPowerState);

	bool IsPowerSupplied();

	bool AreEntriesValid();

	void SetupPowerSuppliers();

	// Reference every object in the scene that will provide power to this interaction (e.g. generators, fans) and the power state they need to be in for the system to provide power.
	UPROPERTY(Instanced, EditInstanceOnly)
	TArray<UPowerSupplierInstance*> PowerSuppliers;

	// Reference every object in the scene that will receive power from this interaction (e.g. doors).
	UPROPERTY(EditAnywhere)
	TArray<APowerReceiver*> PowerReceivers;

	TArray<int32> CurrentPowerStates;

	TArray<int8> RequiredPowerStates;
};
