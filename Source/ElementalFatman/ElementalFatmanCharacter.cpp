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
#include "Materials/MaterialParameterCollection.h"
#include "Components/WidgetComponent.h"
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
	Mesh1P->SetRelativeRotation(FVector(0.f,-90.0f , 0.f).Rotation());
	Mesh1P->SetRelativeLocation(FVector(10.f, 0.f, -20.f));
}

void AElementalFatmanCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	CustomGameModeInstance = Cast<AElementalFatmanGameMode>(GetWorld()->GetAuthGameMode());

	HUD = GetWorld()->GetFirstPlayerController()->GetHUD();
	PlayerController = GetWorld()->GetFirstPlayerController();
}

void AElementalFatmanCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// check if character is moving faster than a speed threshold -- if so, add the "Fast" tag so that the character can break through metal grates
	if (GetVelocity().Size() >= GrateBreakSpeed) { if (!Tags.Contains("Fast")) Tags.Add("Fast"); }
	else { if (Tags.Contains("Fast")) Tags.Remove("Fast"); }

	UpdateMovementBob();

	//////////////////////////////////////////////////////////////////// mantling debug lines

	//// debug line checking mantleable object at foot level
	//FVector TracePoint = FVector(Collider->GetComponentLocation().X, Collider->GetComponentLocation().Y, Collider->GetComponentLocation().Z - GetDefaultHalfHeight());
	//CheckMantle(TracePoint, Collider->GetForwardVector(), DistanceToTriggerMantling, 0);

	//// debug line checking mantleable object at eye level
	//TracePoint.Z += SearchMantleEyeLevel;
	//CheckMantle(TracePoint, Collider->GetForwardVector(), DistanceToTriggerMantling, 0);
	//
	//// debug line checking object is not too tall to mantle
	//CheckMantle(FVector(Collider->GetComponentLocation().X, Collider->GetComponentLocation().Y, (Collider->GetComponentLocation().Z - GetDefaultHalfHeight() + MantleHeightLimit)), Collider->GetForwardVector(), MantleWidthLimit, 1);
	//
	//// debug line checking object is not too thin to mantle
	//FVector TraceDownPoint = FVector(Collider->GetComponentLocation().X, Collider->GetComponentLocation().Y, (Collider->GetComponentLocation().Z - GetDefaultHalfHeight() + MantleHeightLimit)) + (Collider->GetForwardVector() * MantleWidthLimit);
	//CheckMantle(TraceDownPoint, -(Collider->GetUpVector()), 1000, 2);
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

		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AElementalFatmanCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AElementalFatmanCharacter::StopSprinting);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AElementalFatmanCharacter::Look);
	
		// Heating/Cooling
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AElementalFatmanCharacter::Interact);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AElementalFatmanCharacter::CancelInteract);

		// Restart level
		EnhancedInputComponent->BindAction(RestartAction, ETriggerEvent::Started, this, &AElementalFatmanCharacter::Restart);
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

void AElementalFatmanCharacter::Sprint(const FInputActionValue& Value) 
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AElementalFatmanCharacter::StopSprinting(const FInputActionValue& Value) 
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
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

void AElementalFatmanCharacter::Restart(const FInputActionValue& Value) 
{
	PlayerController->RestartLevel();
}


#pragma region Mantling

void AElementalFatmanCharacter::JumpOrMantle() 
{
	if (IsMantling) // if already validated the mantle & character is currently moving, don't run the validity checks again 
	{
		Mantle(MantleStartPos, MantleEndPos);
		return; 
	}

	// new jump/mantle check
	// check if close to mantleable ledge, check both feet level and eye level, either is valid
	bool MantleAtFootLevel = false;
	bool MantleAtEyeLevel = false;

	// searching for mantleable ledge at foot level
	FVector MantleSearchPoint = FVector(Collider->GetComponentLocation().X, Collider->GetComponentLocation().Y, Collider->GetComponentLocation().Z - GetDefaultHalfHeight()); // foot level
	MantleAtFootLevel = CheckMantle(MantleSearchPoint, Collider->GetForwardVector(), DistanceToTriggerMantling);

	// searching for mantleable ledge at eye level
	MantleSearchPoint.Z += SearchMantleEyeLevel; // adjust to eye level
	MantleAtEyeLevel = CheckMantle(MantleSearchPoint, Collider->GetForwardVector(), DistanceToTriggerMantling);

	// if mantleable ledge was found, check if it's valid, then begin mantling, otherwise jump
	if (MantleAtFootLevel || MantleAtEyeLevel) IsMantleValid() ? Mantle(MantleStartPos, MantleEndPos) : Jump();
	else Jump();
}

bool AElementalFatmanCharacter::IsMantleValid()
{
	// check if mantleable object is too tall to climb
	// linetrace from height limit, if hit nothing within width limit, obj not too tall -> can mantle
	if (CheckMantle(FVector(Collider->GetComponentLocation().X, Collider->GetComponentLocation().Y, (Collider->GetComponentLocation().Z - GetDefaultHalfHeight() + MantleHeightLimit)), Collider->GetForwardVector(), MantleWidthLimit))
	{
		UE_LOG(LogTemp, Warning, TEXT("mantle object too tall"));
		return false;
	}

	// check if mantleable object is too thin (or the angle the player is looking at the obj is too extreme) -- essentially ensuring there is a valid point to place the player at
	// from mantle height limit, linetrace down onto mantleable object at the width limit
	FVector TraceDownPoint = FVector(Collider->GetComponentLocation().X, Collider->GetComponentLocation().Y, (Collider->GetComponentLocation().Z - GetDefaultHalfHeight() + MantleHeightLimit)) + (Collider->GetForwardVector() * MantleWidthLimit);
	if (!CheckMantle(TraceDownPoint, -(Collider->GetUpVector()), 1000, 2)) // distance is 1000 bc there's no minimum height limit on mantleable objects
	{
		UE_LOG(LogTemp, Warning, TEXT("mantle object too skinnyyyyy/weird angle"));
		return false;
	}

	// if passed both tests, can mantle! 
	IsMantling = true;
	// set start location for mantle movement (we only want to save this one time)
	MantleStartPos = Collider->GetRelativeLocation();
	// find the point on the mantleable obj we're gonna move the player to (also only want to save this one time)
	MantleEndPos = GetMantleEndPos(TraceDownPoint, -(Collider->GetUpVector()), 1000); // using same linetrace values as in last checkmantle() call
	MantleEndPos.Z += GetDefaultHalfHeight(); // account for the collider's location being its center
	// switch the player mesh collisions with static objects (which all mantleable objects should be???) to overlap so the movement is smoother
	Mesh1P->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);

	// can now proceed with mantling
	return true;
}

bool AElementalFatmanCharacter::CheckMantle(FVector _startPos, FVector _dir, float distance, int debugColorNum)
{
	// linetrace values, record hit & don't hit the character
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FVector StartPos = _startPos;
	FVector Dir = _dir;
	FVector EndPos = StartPos + (Dir * distance);

	DrawDebugLine(GetWorld(), StartPos, EndPos, debugColorNum == 0 ? FColor::Green : debugColorNum == 1 ? FColor::Yellow : FColor::Red, false); // debug line, turn on for bugfixing

	// linetrace using passed values
	if (GetWorld()->LineTraceSingleByChannel(Hit, StartPos, EndPos, ECC_GameTraceChannel2, Params))
	{
		if (!Hit.GetActor()) return false; // if no actor hit, definitely can't mantle
		if (Hit.GetActor()->ActorHasTag("Mantle")) return true; // if hit an actor with mantle tag, can mantle
		return false; // if hit an actor without mantle tag, can't mantle
	}
	return false; // failsafe
}

FVector AElementalFatmanCharacter::GetMantleEndPos(FVector _startPos, FVector _dir, float distance) // virtually identical to checkmantle() but returns a fvector instead of a bool
{
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FVector StartPos = _startPos;
	FVector Dir = _dir;
	FVector EndPos = StartPos + (Dir * distance);

	// not checking validity of linetrace bc this func is only called after checking the exact same linetrace is valid in ismantlevalid()
	GetWorld()->LineTraceSingleByChannel(Hit, StartPos, EndPos, ECC_GameTraceChannel2, Params);
	FVector MantleLocation = Hit.ImpactPoint;
	UE_LOG(LogTemp, Warning, TEXT("ladies and gentlemen, we did it: %f, %f, %f"), MantleLocation.X, MantleLocation.Y, MantleLocation.Z);

	return MantleLocation;
}

void AElementalFatmanCharacter::Mantle(FVector StartPos, FVector EndPos)
{
	UE_LOG(LogTemp, Warning, TEXT("Mantling"));

	// looping on deltatime, call function that moves player to the desired position
	MantleDelegate.BindUFunction(this, "MantleMovement", MantleStartPos, MantleEndPos); // bind function to delegate so it can take params
	GetWorld()->GetTimerManager().SetTimer(MantleHandler, MantleDelegate, GetWorld()->GetTime().GetDeltaWorldTimeSeconds(), true);


	//Collider->SetRelativeLocation(MantlePoint); // instant teleport, can be switched on for bugfixing
}

void AElementalFatmanCharacter::MantleMovement(FVector StartPos, FVector EndPos) 
{
	// increase lerp alpha each frame so character will move over the course of mantleduration seconds
	MantleAlpha += GetWorld()->GetTime().GetDeltaWorldTimeSeconds() / MantleDuration;

	// get the normalised vector value on the curve at current alpha
	// using a curve to get a nice smooth movement from ground to ledge where we mostly move up first, then mostly move forward at the end
	FVector NormalizedNewPos = MantleCurve->GetVectorValue(MantleAlpha);
	
	// de-normalize the curve value so it returns a point between startpos and endpos rather than a point between 0 and 1
	//denormalized_d = normalized_d * (max_d - min_d) + min_d
	FVector NewPos = NormalizedNewPos * (EndPos - StartPos) + StartPos;

	// update the character's position
	Collider->SetRelativeLocation(NewPos);

	if (MantleAlpha >= 1) StopMantling();
}

void AElementalFatmanCharacter::StopJumpingOrMantling()
{
	StopJumping();
	StopMantling();
}

void AElementalFatmanCharacter::StopMantling()
{
	// reset the mantle values & lerp alpha & clear the timer calling the lerp
	IsMantling = false;
	MantleAlpha = 0;
	GetWorld()->GetTimerManager().ClearTimer(MantleHandler);
	Mesh1P->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
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

	LayerInteractionBob();
	//if (GEngine && CurrentInteraction != EInteractionType::null) GEngine->AddOnScreenDebugMessage(-1, 0.01f, CurrentInteraction == EInteractionType::IT_Heating ? FColor::Red : FColor::Blue, FString::Printf(TEXT("%s"), CurrentInteraction == EInteractionType::IT_Heating ? TEXT("Heating") : TEXT("Cooling")));
}

// set interaction enum value
void AElementalFatmanCharacter::CancelInteract(const FInputActionValue& Value)
{
	UpdateInteraction(EInteractionType::null);
	LayerInteractionBob();
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

bool AElementalFatmanCharacter::CheckCrosshairOverInteractable(bool interacting) 
{
	// linetrace parameters
	FHitResult hit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	FVector startPos = FirstPersonCameraComponent->GetComponentLocation();
	FVector dir = FirstPersonCameraComponent->GetForwardVector();
	FVector endPos = startPos + (dir * AbilityRange);


	// line trace, either find a heatinteractable actor or set the currently focused actor to null
	if (GetWorld()->LineTraceSingleByChannel(hit, startPos, endPos, ECC_GameTraceChannel2, params))
	{
		if (Cast<AHeatInteractable>(hit.GetActor()))
		{
			if (interacting) FocusedActor = Cast<AHeatInteractable>(hit.GetActor());
			return true;
		}
		else 
		{
			if (interacting) FocusedActor = nullptr;
			return false;
		}
	}
	else 
	{ 
		if (interacting) FocusedActor = nullptr;
		return false;
	}
}

void AElementalFatmanCharacter::CheckIfHittingInteractable() 
{
	CheckCrosshairOverInteractable(true);
	BeginInteraction();
	LastFocusedActor = FocusedActor;
}

void AElementalFatmanCharacter::BeginInteraction() 
{
	// if not looking at valid actor, stop here & clear the timer
	if (!FocusedActor) { GetWorld()->GetTimerManager().ClearTimer(InteractChargeHandler); return; }

	// check the currently focused actor is the same as whatever player last focused on
	if (FocusedActor == LastFocusedActor)
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

FLinearColor AElementalFatmanCharacter::UpdateCrosshairColor() 
{
	FLinearColor NewColor = CheckCrosshairOverInteractable(false) ? FLinearColor::Green : FLinearColor::White;
	return NewColor;
}

#pragma endregion

void AElementalFatmanCharacter::UpdateMovementBob() 
{
	TSubclassOf<UCameraShakeBase> NewCameraBob = IdleBob;

	if (GetCharacterMovement()->Velocity.Size() > 0)
	{
		if (!GetCharacterMovement()->IsMovingOnGround()) 
		{
			NewCameraBob = FallingBob; 
			//UE_LOG(LogTemp, Warning, TEXT("falling"));
		}
		// else if in landing sequence (will turn off movement and stuff for a sec anyway), landingbob
		else if (GetCharacterMovement()->GetMaxSpeed() == SprintSpeed) 
		{
			//UE_LOG(LogTemp, Warning, TEXT("sprinting"));
			NewCameraBob = SprintingBob;
		}
		else 
		{ 
			//UE_LOG(LogTemp, Warning, TEXT("walking"));
			NewCameraBob = WalkingBob; 
		}
	}
	else 
	{
		//UE_LOG(LogTemp, Warning, TEXT("idle"));
		NewCameraBob = IdleBob;
	}

	if (NewCameraBob != PrevCameraBob) StartCameraShake(NewCameraBob);
	PrevCameraBob = NewCameraBob;
}

void AElementalFatmanCharacter::LayerInteractionBob() 
{
	if (CurrentInteraction != EInteractionType::null) PlayerController->ClientStartCameraShake(InteractBob);
	else PlayerController->ClientStopCameraShake(InteractBob);
}

void AElementalFatmanCharacter::StartCameraShake(TSubclassOf<UCameraShakeBase> NewCameraBob)
{
	PlayerController->ClientStopCameraShake(PrevCameraBob);
	PlayerController->ClientStartCameraShake(NewCameraBob);
}
