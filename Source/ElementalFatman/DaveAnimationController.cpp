// Fill out your copyright notice in the Description page of Project Settings.


#include "DaveAnimationController.h"
#include "Camera\CameraComponent.h"
#include "ElementalFatman.h"
#include "GameFramework\PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UDaveAnimationController::UDaveAnimationController()
{

}

void UDaveAnimationController::NativeBeginPlay()
{
    Super::NativeBeginPlay();

    if (PlayerCharacter = Cast<AElementalFatmanCharacter>(TryGetPawnOwner()))
    {
        UE_LOG(LogPlayerAnimation, Display, TEXT("Found Elementalfatman character"));
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
        bIsMantling = PlayerCharacter->IsMantling;
        CurrentPlayerInteractionType = PlayerCharacter->GetCurrentInteraction();
    }
}

//Actual update code
void UDaveAnimationController::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{

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
        if (IsValid(ArmWalkBobCurve))
        {
            //tweak target point with walk anim curve 
            //Moving
            if (CurrentMoveSpeed >= 5.0) 
            {
                target += ArmWalkBobScalar * ArmWalkBobCurve->GetVectorValue(GetWorld()->GetTimeSeconds()); 
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

#pragma endregion

