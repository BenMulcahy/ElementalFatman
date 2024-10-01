// Fill out your copyright notice in the Description page of Project Settings.


#include "DaveAnimationController.h"
#include "Camera\CameraComponent.h"
#include "ElementalFatman.h"
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

void UDaveAnimationController::UpdateProceduralAnimations()
{
    if (!bUseProceduralAnimations) return; //dont run if not using proc animations

    UpdateTargetHandLocations();
}

FRotator UDaveAnimationController::GetCameraRotationDelta()
{
    FRotator CamRot = PlayerCharacter->GetFirstPersonCameraComponent()->GetComponentRotation();
    FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(CamRot, CachedCameraRot);
    CachedCameraRot = CamRot;
    return delta;
}

void UDaveAnimationController::UpdateTargetHandLocations()
{
    //TODO: Interp to smooth out target locations

    FVector delta = GetCameraRotationDelta().Vector();
    FVector target = FVector(delta.Y * HandSwayMultiplier.X, 0, delta.Z * -HandSwayMultiplier.Y);
    //Update Left Hand Transform based on camera rotation delta
    TargetHandTransform_L.SetLocation(FMath::Lerp(TargetHandTransform_L.GetLocation(), target, 1 - HandIKSmoothing));

    //Update Right Hand
    TargetHandTransform_R.SetLocation(FMath::Lerp(TargetHandTransform_R.GetLocation(), target, 1 -HandIKSmoothing));
}


