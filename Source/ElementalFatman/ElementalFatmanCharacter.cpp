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
#include "HeatInteractable.h"

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
	
		EnhancedInputComponent->BindAction(HeatAction, ETriggerEvent::Triggered, this, &AElementalFatmanCharacter::Heat);

		EnhancedInputComponent->BindAction(ColdAction, ETriggerEvent::Triggered, this, &AElementalFatmanCharacter::Cold);
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

void AElementalFatmanCharacter::Heat(const FInputActionValue& Value) 
{
	// check collision or range 
	bool Heating = Value.Get<bool>();
	if (Heating) 
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, FString::Printf(TEXT("heating")));
		
		// check if youre looking at something based on range
		FHitResult hit;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);

		FVector startPos = FirstPersonCameraComponent->GetForwardVector();
		FVector endPos = FirstPersonCameraComponent->GetForwardVector() * AbilityRange;

		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Magenta, false, 3.5f);

		if (GetWorld()->LineTraceSingleByChannel(hit, startPos, endPos, ECC_GameTraceChannel2, params)) 
		{
			if (Cast<AHeatInteractable>(hit.GetActor())) 
			{
				if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("obj found")));
			}
		}
		// check if object is an interactable Cast<HeatInteractable>??

	}
}

void AElementalFatmanCharacter::Cold(const FInputActionValue& Value)
{
	// check collision or range 
	bool Cooling = Value.Get<bool>();
	UE_LOG(LogTemp, Warning, TEXT("Cooling State: %s"), Cooling ? TEXT("True") : TEXT("False"));
	if (Cooling)
	{

		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, FString::Printf(TEXT("cooling")));
	}
}