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
	UPROPERTY(EditAnywhere)
	UPhysicalMaterial* SlipperyMat;

	UPROPERTY(EditAnywhere)
	UPhysicalMaterial* RoughMat;

protected:

	void Setup();
	void InvokeSpecificMechanic();
};
