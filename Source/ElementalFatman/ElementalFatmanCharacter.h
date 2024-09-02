// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "HeatInteractable.h"
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

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
	/** Interact(Heat/Cool) Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(VisibleAnywhere)
	EInteractionType CurrentInteraction;


public:
	AElementalFatmanCharacter();

protected:
	virtual void BeginPlay();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanics | Abilities", meta = (ClampMin = "0", UIMin = "0"))
	float AbilityRange = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanics | Abilities", meta = (ClampMin = "0", UIMin = "0"))
	float AbilityChargeTime = 1.5f;

	// 1 player pip is actually 1/2 a pip (e.g. hearts)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mechanics | Pips", meta = (ClampMin = "0", UIMin = "0"))
	int32 PlayerPips = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanics | Pips", meta = (ClampMin = "0", UIMin = "0"))
	int32 MaxPlayerPips = 4;

	FTimerHandle InteractChargeHandler;

	AHeatInteractable* FocusedActor;
	AHeatInteractable* LastFocusedActor;


public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AElementalFatmanGameMode> CustomGameModeInstance = nullptr;

	UFUNCTION(BlueprintCallable)
	int32 GetPlayerPips() const { return PlayerPips; }

	UFUNCTION(BlueprintCallable)
	int32 GetMaxPlayerPips() const { return MaxPlayerPips; }

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Interact(const FInputActionValue& Value);
	void CancelInteract(const FInputActionValue& Value);

	void UpdateInteraction(EInteractionType interaction);

	void CheckIfHittingInteractable();

	void BeginInteraction();

	void CompleteInteraction();


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

