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
#include "ElementalFatman.h"
#include "Engine/LocalPlayer.h"
#include "HeatInteractable.h"
#include "ElementalFatmanGameMode.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AElementalFatmanCharacter

AElementalFatmanCharacter::AElementalFatmanCharacter()
{
	// Set size for collision capsule
	Collider = GetCapsuleComponent();
	Collider->InitCapsuleSize(55.f, 96.f);
		
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
	CustomGameModeInstance = Cast<AElementalFatmanGameMode>(GetWorld()->GetAuthGameMode());

	HUD = GetWorld()->GetFirstPlayerController()->GetHUD();
}

void AElementalFatmanCharacter::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);

	FVector TracePoint = FVector(Collider->GetComponentLocation().X, Collider->GetComponentLocation().Y, Collider->GetComponentLocation().Z - GetDefaultHalfHeight());
	CheckMantle(TracePoint, Collider->GetForwardVector(), DistanceToTriggerMantling, 0);

	TracePoint.Z += CheckMantleEyeLevel;
	CheckMantle(TracePoint, Collider->GetForwardVector(), DistanceToTriggerMantling, 0);

	CheckMantle(FVector(Collider->GetComponentLocation().X, Collider->GetComponentLocation().Y, UpperMantleLimit), Collider->GetForwardVector(), InnerMantleLimit, 1);
	
	FVector TraceDownPoint = FVector(Collider->GetComponentLocation().X, Collider->GetComponentLocation().Y, UpperMantleLimit) + (Collider->GetForwardVector() * InnerMantleLimit);
	CheckMantle(TraceDownPoint, -(Collider->GetUpVector()), 1000, 2);
}

//////////////////////////////////////////////////////////////////////////// Input

void AElementalFatmanCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AElementalFatmanCharacter::JumpOrMantle);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AElementalFatmanCharacter::StopJumpingOrMantling);

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

#pragma region Mantling

void AElementalFatmanCharacter::JumpOrMantle() 
{
	// check if close to mantleable ledge, check both feet level and eye level, either is valid
	bool MantleAtFootLevel = false;
	bool MantleAtEyeLevel = false;

	FVector TracePoint = FVector(Collider->GetComponentLocation().X, Collider->GetComponentLocation().Y, Collider->GetComponentLocation().Z - GetDefaultHalfHeight());
	MantleAtFootLevel = CheckMantle(TracePoint, Collider->GetForwardVector(), DistanceToTriggerMantling);

	TracePoint.Z += CheckMantleEyeLevel; // adjust for eye level
	MantleAtEyeLevel = CheckMantle(TracePoint, Collider->GetForwardVector(), DistanceToTriggerMantling);

	if (MantleAtFootLevel || MantleAtEyeLevel) IsMantleValid() ? Mantle() : Jump();
}

void AElementalFatmanCharacter::StopJumpingOrMantling() 
{
	StopJumping();
	StopMantling();
}

bool AElementalFatmanCharacter::IsMantleValid()
{
	// check if mantleable object is too tall
	// linetrace from upper limit, if hit nothing within inner limit distance, obj not too tall -> can mantle
	if (CheckMantle(FVector(Collider->GetComponentLocation().X, Collider->GetComponentLocation().Y, UpperMantleLimit), Collider->GetForwardVector(), InnerMantleLimit))
	{
		UE_LOG(LogTemp, Warning, TEXT("mantle object too tall"));
		return false;
	}

	// check if mantleable object is too thin (or the angle the player is looking at the obj is too extreme)
	// from upper mantle limit, linetrace down onto mantleable object at the inner limit distance
	FVector TraceDownPoint = FVector(Collider->GetComponentLocation().X, Collider->GetComponentLocation().Y, UpperMantleLimit) + (Collider->GetForwardVector() * InnerMantleLimit);
	if (!CheckMantle(TraceDownPoint, -(Collider->GetUpVector()), 1000, 2))
	{
		UE_LOG(LogTemp, Warning, TEXT("mantle object too skinnyyyyy/weird angle"));
		return false;
	}

	// if passed both tests, can mantle
	return true;
}

bool AElementalFatmanCharacter::CheckMantle(FVector _startPos, FVector _dir, float distance, int colorNum)
{
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FVector StartPos = _startPos;
	FVector Dir = _dir;
	FVector EndPos = StartPos + (Dir * distance);

	DrawDebugLine(GetWorld(), StartPos, EndPos, colorNum == 0 ? FColor::Green : colorNum == 1 ? FColor::Yellow : FColor::Red, false);

	if (GetWorld()->LineTraceSingleByChannel(Hit, StartPos, EndPos, ECC_GameTraceChannel2, Params))
	{
		if (!Hit.GetActor()) return false;
		if (Hit.GetActor()->ActorHasTag("Mantle")) return true;
		return false;
	}
	return false;
}

void AElementalFatmanCharacter::Mantle()
{
	UE_LOG(LogTemp, Warning, TEXT("Mantling"));

	// find the point on the mantleable obj we're gonna move the player to
	FVector MantlePoint = GetMantlePoint();
	MantlePoint.Z += GetDefaultHalfHeight(); // account for the collider's position being its center

	// set a kind of "stick to object" bool, move player up based on a duration time
	// move player forwards/on based on a duration time

	Collider->SetRelativeLocation(MantlePoint); // temp instant teleport, make lerp/animation later
	// switch off the "stick to object" bool
}

FVector AElementalFatmanCharacter::GetMantlePoint() 
{
	FVector MantleLocation;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FVector StartPos = FVector(Collider->GetComponentLocation().X, Collider->GetComponentLocation().Y, UpperMantleLimit) + (Collider->GetForwardVector() * InnerMantleLimit);
	FVector Dir = -(Collider->GetUpVector());
	FVector EndPos = StartPos + (Dir * 1000);

	// not checking validity of line trace bc we just did that with validatemantle()
	GetWorld()->LineTraceSingleByChannel(Hit, StartPos, EndPos, ECC_GameTraceChannel2, Params);
	MantleLocation = Hit.ImpactPoint;
	UE_LOG(LogTemp, Warning, TEXT("ladies and gentlemen, we did it: %f, %f, %f"), MantleLocation.X, MantleLocation.Y, MantleLocation.Z);
	
	return MantleLocation;
}

void AElementalFatmanCharacter::StopMantling()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop mantling"));
	// switch off the "stick to object" bool
}

#pragma endregion

#pragma region Interacting

// set interaction enum value
void AElementalFatmanCharacter::Interact(const FInputActionValue& Value) 
{
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, FString::Printf(TEXT("heat: %f, cool: %f"), Value.Get<FVector2D>().X, Value.Get<FVector2D>().Y));

	if (Value.Get<FVector2D>().X > 0 && Value.Get<FVector2D>().Y == 0)
	{
		UpdateInteraction(EInteractionType::IT_Heating);
	}
	else if (Value.Get<FVector2D>().Y > 0 && Value.Get<FVector2D>().X == 0) 
	{
		UpdateInteraction(EInteractionType::IT_Cooling);
	}
	else
	{
		//Retain first interaction input in case both inputs are given
		UpdateInteraction(CurrentInteraction);
	}

	//if (GEngine && CurrentInteraction != EInteractionType::null) GEngine->AddOnScreenDebugMessage(-1, 0.01f, CurrentInteraction == EInteractionType::IT_Heating ? FColor::Red : FColor::Blue, FString::Printf(TEXT("%s"), CurrentInteraction == EInteractionType::IT_Heating ? TEXT("Heating") : TEXT("Cooling")));
}

// set interaction enum value
void AElementalFatmanCharacter::CancelInteract(const FInputActionValue& Value)
{
	UpdateInteraction(EInteractionType::null);
}

void AElementalFatmanCharacter::UpdateInteraction(EInteractionType interaction)
{
	CurrentInteraction = interaction;

	// switch through interaction cases, update any interactables you're hitting
	switch (interaction)
	{
	case EInteractionType::null:
		UE_LOG(LogPlayer, Log, TEXT("Interact cancelled"));
		// when player cancels input, clear the interaction timer
		GetWorld()->GetTimerManager().ClearTimer(InteractChargeHandler);
 		break;
	case EInteractionType::IT_Heating:
		CheckIfHittingInteractable();
		break;
	case EInteractionType::IT_Cooling:
		CheckIfHittingInteractable();
		break;
	default:
		break;
	}
}

void AElementalFatmanCharacter::CheckIfHittingInteractable() 
{
	// linetrace parameters
	FHitResult hit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	FVector startPos = FirstPersonCameraComponent->GetComponentLocation();
	FVector dir = FirstPersonCameraComponent->GetForwardVector();
	FVector endPos = startPos + (dir * AbilityRange);

	DrawDebugLine(GetWorld(), startPos, endPos, FColor::Magenta, false);

	// line trace, either find a heatinteractable actor or set the currently focused actor to null
	if (GetWorld()->LineTraceSingleByChannel(hit, startPos, endPos, ECC_GameTraceChannel2, params))
	{
		if (Cast<AHeatInteractable>(hit.GetActor()))
		{
			FocusedActor = Cast<AHeatInteractable>(hit.GetActor());
			// update crosshair colour
		}
		else FocusedActor = nullptr; 
	}
	else FocusedActor = nullptr;

	BeginInteraction();
	LastFocusedActor = FocusedActor;
}

void AElementalFatmanCharacter::BeginInteraction() 
{
	// if not looking at valid actor, stop here & clear the timer
	if (!FocusedActor) GetWorld()->GetTimerManager().ClearTimer(InteractChargeHandler);

	// check the currently focused actor is the same as whatever player last focused on
	else if (FocusedActor == LastFocusedActor)
	{
		// if the timer hasn't already started, start the timer			
		if (!GetWorld()->GetTimerManager().IsTimerActive(InteractChargeHandler))
		{
			// on timer completion, attempt to heat/cool the focused actor
			GetWorld()->GetTimerManager().SetTimer(InteractChargeHandler, this, &AElementalFatmanCharacter::CompleteInteraction, AbilityChargeTime, false);
			UE_LOG(LogPlayer, Warning, TEXT("started timer"));
		}
	}
	else
	{
		// if currently focused actor is not the same as whatever player last focused on, clear the timer
		GetWorld()->GetTimerManager().ClearTimer(InteractChargeHandler);
		UE_LOG(LogPlayer, Warning, TEXT("new focused actor"));
	}
}

void AElementalFatmanCharacter::CompleteInteraction() 
{
	UE_LOG(LogPlayer, Warning, TEXT("completed timer"));

	// clear the timer
	GetWorld()->GetTimerManager().ClearTimer(InteractChargeHandler);

	// attempt to heat/cool the actor and lose/gain a pip
	int32 pipDiff = FocusedActor->ValidateInteraction(CurrentInteraction == EInteractionType::IT_Heating ? true : false, CurrentPlayerPips, MaxPlayerPips);
	CurrentPlayerPips += pipDiff;

	// call player pip ui update here please
}

#pragma endregion

