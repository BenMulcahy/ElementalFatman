// Copyright Epic Games, Inc. All Rights Reserved.

#include "ElementalFatmanCharacter.h"
#include "ElementalFatmanProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "ElementalFatmanGameMode.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AElementalFatmanCharacter

AElementalFatmanCharacter::AElementalFatmanCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	// CreateDefaultSubobject creates the component on a blueprint before the game even runs
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void AElementalFatmanCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	CustomGameModeInstance = CastChecked<AElementalFatmanGameMode>(GetWorld()->GetAuthGameMode());
}

//////////////////////////////////////////////////////////////////////////// Input

void AElementalFatmanCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AElementalFatmanCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AElementalFatmanCharacter::Look);
	
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AElementalFatmanCharacter::Interact);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AElementalFatmanCharacter::CancelInteract);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AElementalFatmanCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AElementalFatmanCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

#pragma region Interacting

// set interaction enum value
void AElementalFatmanCharacter::Interact(const FInputActionValue& Value) 
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, FString::Printf(TEXT("heat: %f, cool: %f"), Value.Get<FVector2D>().X, Value.Get<FVector2D>().Y));

	if (Value.Get<FVector2D>().X > 0 && Value.Get<FVector2D>().Y == 0)
	{
		UpdateInteraction(EInteractionType::IT_Heating);
	}
	else if (Value.Get<FVector2D>().Y > 0 && Value.Get<FVector2D>().X == 0) {
		UpdateInteraction(EInteractionType::IT_Cooling);
	}
	else
	{
		//Retain first interaction input in case both inputs are given
		UpdateInteraction(CurrentInteraction);
	}

	if (GEngine && CurrentInteraction != EInteractionType::null) GEngine->AddOnScreenDebugMessage(-1, 0.01f, CurrentInteraction == EInteractionType::IT_Heating ? FColor::Red : FColor::Blue, FString::Printf(TEXT("%s"), CurrentInteraction == EInteractionType::IT_Heating ? TEXT("Heating") : TEXT("Cooling")));
}

// set interaction enum value
void AElementalFatmanCharacter::CancelInteract(const FInputActionValue& Value)
{
	UpdateInteraction(EInteractionType::null);
}

void AElementalFatmanCharacter::UpdateInteraction(EInteractionType interaction)
{
	CurrentInteraction = interaction;
	
	// reset everything first
	ResetAllInteractables();

	// switch through interaction cases, update any interactables you're hitting
	switch (interaction)
	{
	case EInteractionType::null:
		UE_LOG(LogPlayerManagement, Log, TEXT("InteractCancelled"));
 		break;
	case EInteractionType::IT_Heating:
		UpdateHitInteractable(true);
		break;
	case EInteractionType::IT_Cooling:
		UpdateHitInteractable(false);
		break;
	default:
		break;
	}
}

void AElementalFatmanCharacter::UpdateHitInteractable(bool heating) 
{
	// linetrace parameters
	FHitResult hit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	FVector startPos = FirstPersonCameraComponent->GetComponentLocation();
	FVector dir = FirstPersonCameraComponent->GetForwardVector();
	FVector endPos = startPos + (dir * AbilityRange);

	//DrawDebugLine(GetWorld(), startPos, endPos, FColor::Magenta, false);

	if (GetWorld()->LineTraceSingleByChannel(hit, startPos, endPos, ECC_GameTraceChannel2, params))
	{		
		// find a heatinteractable actor, set its interacted state & change its mesh color depending on whether heating or cooling
		if (AHeatInteractable* hitActor = Cast<AHeatInteractable>(hit.GetActor()))
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, FString::Printf(TEXT("interactable found")));
			hitActor->SetInteractedState(true);
			hitActor->ChangeColor(heating ? FColor::Red : FColor::Blue);
		}
	}
}

void AElementalFatmanCharacter::ResetAllInteractables()
{
	for (AActor* actor : CustomGameModeInstance->GetSceneInteractables())
	{
		AHeatInteractable* interactable = Cast<AHeatInteractable>(actor);
		interactable->ChangeColor(FColor::Purple);
		interactable->SetInteractedState(false);
	}
}
#pragma endregion

