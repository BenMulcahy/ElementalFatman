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

	/// <summary>
	/// Collect data here
	/// </summary>
	/// <param name="DeltaSeconds"></param>
	virtual void NativeUpdateAnimation(float DeltaSeconds);

	/// <summary>
	/// Actual Update work
	/// </summary>
	/// <param name="DeltaSeconds"></param>
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);

	

#pragma region Vars
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AElementalFatmanCharacter> PlayerCharacter;

	UPROPERTY(BlueprintReadOnly)
	float CurrentMoveSpeed = 0.f;

	UPROPERTY(BlueprintReadOnly)
	bool bIsFalling;

	UPROPERTY(BlueprintReadOnly)
	bool bIsMantling;

	UPROPERTY(BlueprintReadOnly)
	EInteractionType CurrentPlayerInteractionType;

	UPROPERTY()
	FRotator CachedCameraRot;

	UPROPERTY(BlueprintReadWrite)
	FTransform TargetHandTransform_L;

	UPROPERTY(BlueprintReadWrite)
	FTransform TargetHandTransform_R;

	UPROPERTY(EditAnywhere)
	FVector2f HandSwayMultiplier = FVector2f(25.0,50);

	UPROPERTY(EditAnywhere)
	float HandSwayInteractionFactor = 0.33f;

	/// <summary>
	/// Distance at which players hands begin to brace themselves up against the wall
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HandWallBraceDistance = 70.f;

	UPROPERTY(BlueprintReadOnly)
	bool bUseProceduralAnimations;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCurveVector> ArmWalkBobCurve;

	UPROPERTY(EditAnywhere)
	float ArmWalkBobScalar = 1.f;

	UPROPERTY(EditAnywhere)
	float ArmIdleBobScalar = 0.33f;

	UPROPERTY(EditAnywhere)
	float FallingIKScalar = 0.25f;

	UPROPERTY(EditAnywhere)
	FVector2D FallingIKVelocityRange = FVector2D(0, 10);
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "0.9", UIMin = "0", UIMax = "1"))
	float HandIKSmoothing = 0.85f;

	UPROPERTY(EditAnywhere)
	float ProceduralAnimRate = 0.0167f;

	UPROPERTY(BlueprintReadOnly)
	float DistToWall = 0.f;

	FHitResult WallHit;
	float InteractionDuration;

private:
	FTimerHandle ProceduralAnimationTimer;

#pragma endregion


#pragma region Functions
protected:

	///Get the delta between the camera rotation this frame compared to last frame
	UFUNCTION()
	FRotator GetCameraRotationDelta();	

	/// <summary>
	/// Set Target Hand Location for IK solvers
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void UpdateTargetHandLocations();

	/// <summary>
	/// Raycast to wall and check if hands should be pushed up to wall
	/// </summary>
	/// <returns></returns>
	UFUNCTION()
	bool IsAgainstWall();

	/// <summary>
	/// Measure duration of heating or cold interactions
	/// </summary>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable)
	float InteractionTimer();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void ToggleProceduralAnimations()
	{
		bUseProceduralAnimations = !bUseProceduralAnimations; 
		UE_LOG(LogPlayerAnimation, Warning, TEXT("Procedural Player Animations: %s"), bUseProceduralAnimations ? TEXT("On") : TEXT("Off"));
	}

#pragma endregion
};
