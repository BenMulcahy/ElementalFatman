// Fill out your copyright notice in the Description page of Project Settings.


#include "Torch.h"

void ATorch::Setup()
{
	if (OverrideMesh)
	{
		// replace mesh with variants here
	}

	UE_LOG(LogInteraction, Warning, TEXT("setting up torch"));

	ObjectType = EObjectType::OT_Torch;

	if (!GetComponentByClass<ULocalLightComponent>()) { UE_LOG(LogTemp, Error, TEXT("Every torch MUST have a light component! Interacting with this torch will crash the editor!")); }
	else 
	{
		LightComponent = GetComponentByClass<ULocalLightComponent>();
		LightIntensity = LightComponent->Intensity;
		LightSize = LightComponent->AttenuationRadius;
	}

	if (!HasEmbers) EmbersIntensity = 0;

	MaxInteractablePips = 1;

	if (StartLit) 
	{
		CurrentInteractablePips = 1;
		LightComponent->SetIntensity(LightIntensity);
		LightComponent->SetAttenuationRadius(LightSize);
	}
	else 
	{
		CurrentInteractablePips = 0;
		LightComponent->SetIntensity(EmbersIntensity);
		LightComponent->SetAttenuationRadius(100);
	}

	Super::Setup();
}


void ATorch::InvokeSpecificMechanic()
{
	FTimerDelegate FadeDelegate;

	switch (CurrentInteractablePips)
	{
	case 0: // switch off
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("torch turned off")));

		FadeDelegate.BindUFunction(this, "FadeIntensity", false);
		FadeAlpha = 1;
		GetWorld()->GetTimerManager().SetTimer(FadeHandle, FadeDelegate, GetWorld()->DeltaTimeSeconds, true); 

		break;
	case 1: // switch on
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("torch turned on")));

		FadeDelegate.BindUFunction(this, "FadeIntensity", true);
		FadeAlpha = 0;
		GetWorld()->GetTimerManager().SetTimer(FadeHandle, FadeDelegate, GetWorld()->DeltaTimeSeconds, true);

		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Torch current pip value error!"));
		break;
	}
}

void ATorch::FadeIntensity(bool increasing)
{
	// finish increasing/decreasing the glow once the alpha reaches its desired end value
	if (increasing ? FadeAlpha >= 1 : FadeAlpha <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(FadeHandle);
		LightComponent->SetIntensity(increasing ? LightIntensity : EmbersIntensity); // ensure the end value is exact
		LightComponent->SetAttenuationRadius(increasing ? LightSize : 100);
		return;
	}

	// change the alpha value linearly between 0 and 1 (or 1 and 0 if fading out)
	FadeAlpha += GetWorld()->DeltaTimeSeconds / (increasing ? FadeDuration : -FadeDuration);

	//denormalized_d = normalized_d * (max_d - min_d) + min_d
	float NewIntensity = FadeAlpha * (LightIntensity - EmbersIntensity) + EmbersIntensity;
	float NewSize = FadeAlpha * (LightSize - 100) + 100;

	// update the glow light's intensity
	LightComponent->SetIntensity(NewIntensity);
	LightComponent->SetAttenuationRadius(NewSize);
}