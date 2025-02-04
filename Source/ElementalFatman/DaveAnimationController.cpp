// Fill out your copyright notice in the Description page of Project Settings.


#include "DaveAnimationController.h"
#include "Camera\CameraComponent.h"
#include "ElementalFatman.h"
#include "GameFramework\PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#if WITH_EDITOR
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h" 
#endif // WITH_EDITOR


UDaveAnimationController::UDaveAnimationController()
{

}

void UDaveAnimationController::NativeBeginPlay()
{
    Super::NativeBeginPlay();

    if (PlayerCharacter = Cast<AElementalFatmanCharacter>(TryGetPawnOwner()))
    {
        UE_LOG(LogPlayerAnimation, Log, TEXT("Found Elementalfatman character"));
        CachedCameraRot = PlayerCharacter->GetFirstPersonCameraComponent()->GetComponentRotation();
    }
    else UE_LOG(LogPlayerAnimation, Fatal, TEXT("Dave anim can't find character/is being used incorrectly?"));

    GetWorld()->GetTimerManager().SetTimer(ProceduralAnimationTimer, this, &UDaveAnimationController::UpdateProceduralAnimations, ProceduralAnimRate, true);
}

#pragma region Updates
//Data collation
void UDaveAnimationController::NativeUpdateAnimation(float DeltaSeconds)
{
    if (PlayerCharacter)
    {
        CurrentMoveSpeed = PlayerCharacter->GetMovementComponent()->Velocity.Length();
        bIsFalling = PlayerCharacter->GetMovementComponent()->IsFalling();
        bIsMantling = PlayerCharacter->GetIsMantling();
        CurrentPlayerInteractionType = PlayerCharacter->GetCurrentInteraction();
    }
}

//Actual update code
void UDaveAnimationController::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
    InteractionTimer();
}

float UDaveAnimationController::InteractionTimer()
{
    if (CurrentPlayerInteractionType != EInteractionType::null)
    {
        InteractionDuration += GetWorld()->DeltaTimeSeconds;
    }
    else InteractionDuration = 0;

    return InteractionDuration;
}

#pragma endregion

#pragma region Proceduaral Animations

void UDaveAnimationController::UpdateProceduralAnimations()
{
    if (!bUseProceduralAnimations) return; //dont run if not using procedrual animations

    UpdateTargetHandLocations();
}

FRotator UDaveAnimationController::GetCameraRotationDelta()
{
    FRotator CamRot = PlayerCharacter->GetFirstPersonCameraComponent()->GetComponentRotation();
    FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(CamRot, CachedCameraRot);
    
    //Todo: move to save end of update/frame
    CachedCameraRot = CamRot;
    return delta;
}



void UDaveAnimationController::UpdateTargetHandLocations()
{
    FVector delta = GetCameraRotationDelta().Vector();
    FVector target = FVector(delta.Y * HandSwayMultiplier.X, 0, delta.Z * -HandSwayMultiplier.Y); //set target point based on camera rotation delta

    //On ground
    if (!bIsFalling)
    {
        if (IsAgainstWall())
        {
            //Debug hit point
            //if (GEngine) DrawDebugSphere(GetWorld(), WallHit.ImpactPoint, 3.f, 12, FColor::Red, false, 5.f, 0, 0.5f);
            
            //Adjust Target Y closer to player if near a wall
            target.Y += UKismetMathLibrary::MapRangeClamped(DistToWall, 0, HandWallBraceDistance, 15.f, 5);
        }
        else if (IsValid(ArmWalkBobCurve))
        {
            //tweak target point with walk anim curve 
            //Moving
            if (CurrentMoveSpeed >= 5.0)
            {
                target += ArmWalkBobScalar * (CurrentPlayerInteractionType == EInteractionType::null ? 1 : HandSwayInteractionFactor) * ArmWalkBobCurve->GetVectorValue(GetWorld()->GetTimeSeconds());
            }
            //Idle
            else target += ArmIdleBobScalar * ArmWalkBobCurve->GetVectorValue(GetWorld()->GetTimeSeconds() * 0.2);
        }
        else UE_LOG(LogPlayerAnimation, Warning, TEXT("No Walk Curve!"));
    }
    else //falling
    {
        //Set IK Target Z point based on players falling velocity, mapped to a given range
        TObjectPtr<UMovementComponent> playerMvmnt = PlayerCharacter->GetMovementComponent();
        target.Z = UKismetMathLibrary::MapRangeClamped(-playerMvmnt->Velocity.Z * FallingIKScalar, 0, playerMvmnt->GetMaxSpeed(), FallingIKVelocityRange.X, FallingIKVelocityRange.Y);
    }

    //Set Left Hand target point
    TargetHandTransform_L.SetLocation(FMath::Lerp(TargetHandTransform_L.GetLocation(), target, 1 - HandIKSmoothing));

    //Set Right Hand target point
    TargetHandTransform_R.SetLocation(FMath::Lerp(TargetHandTransform_R.GetLocation(), target, 1 -HandIKSmoothing));
}

bool UDaveAnimationController::IsAgainstWall()
{
    FHitResult hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(PlayerCharacter);

    FVector StartPos = PlayerCharacter->GetPawnViewLocation();
    FVector Dir = PlayerCharacter->GetFirstPersonCameraComponent()->GetForwardVector();
    FVector EndPos = StartPos + (Dir * HandWallBraceDistance);

    if (GetWorld()->LineTraceSingleByChannel(hit, StartPos, EndPos, ECC_GameTraceChannel2, Params))
    {
        WallHit = hit; 

        //Get Dist to wall
        DistToWall = (PlayerCharacter->GetPawnViewLocation() - WallHit.ImpactPoint).Length();

        //UE_LOG(LogPlayerAnimation, Display, TEXT("Dist to wall: %f"), DistToWall);
        return true;
    }
    else
    {
        DistToWall = 0;
        return false;
    }
}

#pragma endregion

