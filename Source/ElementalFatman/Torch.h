// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeatInteractable.h"
#include "Components/LightComponent.h"
#include "Torch.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTALFATMAN_API ATorch : public AHeatInteractable
{
	GENERATED_BODY()

protected:

	void Setup();
	void InvokeSpecificMechanic();

	UFUNCTION()
	void FadeIntensity(bool increasing);

	UPROPERTY(EditAnywhere)
	float FadeDuration = 0.2f;

	FTimerHandle FadeHandle;
	float FadeAlpha;

	// Does this torch start the game lit?
	UPROPERTY(EditAnywhere)
	bool StartLit = false;

	// Does this light source have a "glowing coals" effect when unlit/cooled?
	UPROPERTY(EditAnywhere)
	bool HasEmbers = true;

	// Intensity of the "glowing coals" light.
	UPROPERTY(EditAnywhere, meta=(EditCondition="HasEmbers", EditConditionHides))
	float EmbersIntensity;

	float LightIntensity;

	ULightComponent* LightComponent = nullptr;
};
