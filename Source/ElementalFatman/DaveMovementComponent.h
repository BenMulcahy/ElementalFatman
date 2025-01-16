// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DaveMovementComponent.generated.h"

UENUM()
enum ECustomMovementModes : int
{
	MOVE_Land		UMETA(DisplayName = "Landing"),
	MOVE_Sprint		UMETA(DisplayName = "Sprinting"),
	MOVE_Idle		UMETA(DisplayName = "Idling"),
};

UCLASS()
class ELEMENTALFATMAN_API UDaveMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode);
	void UpdateMovementMode();
};
