// Fill out your copyright notice in the Description page of Project Settings.


#include "DaveMovementComponent.h"

void UDaveMovementComponent::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	FText DefaultModeText;
	UEnum::GetDisplayValueAsText(PrevMovementMode, DefaultModeText);

	if (PreviousCustomMode != 0) { UE_LOG(LogTemp, Warning, TEXT("%d"), PreviousCustomMode); }
	else UE_LOG(LogTemp, Warning, TEXT("%s"), *DefaultModeText.ToString());
}

void UDaveMovementComponent::UpdateMovementMode() 
{
	if (Velocity.Size() > 0)
	{
		// else if in landing sequence (will turn off movement and stuff for a sec anyway), MOVE_Land
		if (GetMaxSpeed() == 0 /*SprintSpeed*/)
		{
			SetMovementMode(MOVE_Custom, MOVE_Sprint);
		}
		else
		{
			 SetMovementMode(MOVE_Walking);
		}
	}
	else
	{
		SetMovementMode(MOVE_Custom, MOVE_Idle);
	}
}

