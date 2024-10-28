// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeatInteractable.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "MovableBox.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTALFATMAN_API AMovableBox : public AHeatInteractable
{
	GENERATED_BODY()

public:

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UPhysicalMaterial* SlipperyMat;

	UPROPERTY(EditAnywhere)
	UPhysicalMaterial* RoughMat;

	UPROPERTY(EditAnywhere)
	bool StartFrozen = false;

	UPROPERTY(EditAnywhere, Category = "Mechanics | Breaking Grates")
	bool CanBreakGrates = true;

	UPROPERTY(EditAnywhere, Category = "Mechanics | Breaking Grates", meta = (EditCondition = "CanBreakGrates", EditConditionHides))
	float GrateBreakSpeed = 500.f;

protected:

	void Setup();
	void InvokeSpecificMechanic();
};
