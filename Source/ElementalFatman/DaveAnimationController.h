// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ElementalFatmanCharacter.h"
#include "DaveAnimationController.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTALFATMAN_API UDaveAnimationController : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UDaveAnimationController();

protected:
	virtual void NativeBeginPlay();
	void UpdateProceduralAnimations();

#pragma region Vars
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AElementalFatmanCharacter> PlayerCharacter;

	UPROPERTY()
	FRotator CachedCameraRot;

	UPROPERTY(BlueprintReadWrite)
	FTransform TargetHandTransform_L;

	UPROPERTY(BlueprintReadWrite)
	FTransform TargetHandTransform_R;

	UPROPERTY(EditAnywhere)
	FVector2f HandSwayMultiplier = FVector2f(25.0,50);

	UPROPERTY(EditAnywhere)
	bool bUseProceduralAnimations;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "0.9", UIMin = "0", UIMax = "1"))
	float HandIKSmoothing = 0.85f;

	UPROPERTY(EditAnywhere)
	float ProceduralAnimRate = 0.0167f;

private:
	FTimerHandle ProceduralAnimationTimer;

#pragma endregion


#pragma region Functions
protected:

	UFUNCTION(BlueprintCallable)
	FRotator GetCameraRotationDelta();	

	UFUNCTION(BlueprintCallable)
	void UpdateTargetHandLocations();

#pragma endregion
};
