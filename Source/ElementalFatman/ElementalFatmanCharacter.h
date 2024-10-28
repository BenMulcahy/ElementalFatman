// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "HeatInteractable.h"
#include "Curves/CurveVector.h"
#include "ElementalFatmanCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UENUM()
enum class EInteractionType : uint8
{
	null UMETA(DisplayName = "No Interaction"),
	IT_Heating UMETA(DisplayName = "Heating Interaction"),
	IT_Cooling UMETA(DisplayName = "Cooling Interaction"),
};

UCLASS(config=Game)
class AElementalFatmanCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh | Character", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, Category = Collider)
	UCapsuleComponent* Collider;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
	/** Interact(Heat/Cool) Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/** Restart Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RestartAction;

	UPROPERTY(VisibleAnywhere)
	EInteractionType CurrentInteraction;


public:
	AElementalFatmanCharacter();

protected:
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanics | Abilities", meta = (ClampMin = "0", UIMin = "0"))
	float AbilityRange = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanics | Abilities", meta = (ClampMin = "0", UIMin = "0"))
	float AbilityChargeTime = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanics | Pips", meta = (ClampMin = "0", UIMin = "0"))
	int32 CurrentPlayerPips = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanics | Pips", meta = (ClampMin = "0", UIMin = "0"))
	int32 MaxPlayerPips = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanics | Mantling", meta = (ClampMin = "0", UIMin = "0"))
	float DistanceToTriggerMantling = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanics | Mantling", meta = (ClampMin = "0", UIMin = "0"))
	float SearchMantleEyeLevel = 150;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanics | Mantling", meta = (ClampMin = "0", UIMin = "0"))
	float MantleHeightLimit = 250;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanics | Mantling", meta = (ClampMin = "0", UIMin = "0"))
	float MantleWidthLimit = 75;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanics | Mantling", meta = (ClampMin = "0", UIMin = "0"))
	float MantleDuration = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanics | Mantling", meta = (ClampMin = "0", UIMin = "0"))
	UCurveVector* MantleCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanics | Breaking Grates", meta = (ClampMin = "0", UIMin = "0"))
	float GrateBreakSpeed = 500.f;
	
	FTimerDelegate MantleDelegate;

	FTimerHandle MantleHandler;
	FTimerHandle InteractChargeHandler;

	AHeatInteractable* FocusedActor;
	AHeatInteractable* LastFocusedActor;

	AHUD* HUD;

	float CharacterGravity;
	UPROPERTY(VisibleAnywhere)
	FVector MantleStartPos;
	FVector MantleEndPos;
	float MantleAlpha;

public:
	UPROPERTY(VisibleAnywhere)
	bool IsMantling;
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AElementalFatmanGameMode> CustomGameModeInstance = nullptr;

	UFUNCTION(BlueprintCallable)
	int32 GetPlayerPips() const { return CurrentPlayerPips; }

	UFUNCTION(BlueprintCallable)
	int32 GetMaxPlayerPips() const { return MaxPlayerPips; }

	UFUNCTION(BlueprintCallable)
	EInteractionType GetCurrentInteraction() const { return CurrentInteraction; }

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Restart(const FInputActionValue& Value);

	void JumpOrMantle();

	bool CheckMantle(FVector _startPos, FVector _dir, float distance, int colorNum = 0);

	bool IsMantleValid();

	FVector GetMantleEndPos(FVector _startPos, FVector _dir, float distance);

	UFUNCTION()
	void MantleMovement(FVector StartPos, FVector EndPos);

	void StopJumpingOrMantling();

	void Mantle(FVector StartPos, FVector EndPos);

	void StopMantling();

	void Interact(const FInputActionValue& Value);
	void CancelInteract(const FInputActionValue& Value);

	void UpdateInteraction(EInteractionType interaction);

	void CheckIfHittingInteractable();

	void BeginInteraction();

	void CompleteInteraction();

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
};

