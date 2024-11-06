// Fill out your copyright notice in the Description page of Project Settings.


#include "Lamp.h"

void ALamp::BeginPlay() 
{
	Super::BeginPlay();

	if (!GetComponentByClass<ULightComponent>()) { UE_LOG(LogTemp, Error, TEXT("Every torch MUST have a light component! Interacting with this torch will crash the editor!")); }
	else
	{
		LightComponent = GetComponentByClass<ULightComponent>();
		LightIntensity = LightComponent->Intensity;
	}

	if (!HasEmbers) EmbersIntensity = 0;

	if (StartLit) LightComponent->SetIntensity(LightIntensity);
	else LightComponent->SetIntensity(EmbersIntensity);
}

void ALamp::Activate() 
{
	FTimerDelegate FadeDelegate;

	FadeDelegate.BindUFunction(this, "FadeIntensity", true);
	FadeAlpha = 0;
	GetWorld()->GetTimerManager().SetTimer(FadeHandle, FadeDelegate, GetWorld()->DeltaTimeSeconds, true);
}

void ALamp::Deactivate() 
{
	FTimerDelegate FadeDelegate;

	FadeDelegate.BindUFunction(this, "FadeIntensity", false);
	FadeAlpha = 1;
	GetWorld()->GetTimerManager().SetTimer(FadeHandle, FadeDelegate, GetWorld()->DeltaTimeSeconds, true);
}

void ALamp::FadeIntensity(bool increasing) 
{
	// finish increasing/decreasing the glow once the alpha reaches its desired end value
	if (increasing ? FadeAlpha >= 1 : FadeAlpha <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(FadeHandle);
		LightComponent->SetIntensity(increasing ? LightIntensity : EmbersIntensity); // ensure the end value is exact
		return;
	}

	// change the alpha value linearly between 0 and 1 (or 1 and 0 if fading out)
	FadeAlpha += GetWorld()->DeltaTimeSeconds / (increasing ? FadeDuration : -FadeDuration);

	//denormalized_d = normalized_d * (max_d - min_d) + min_d
	float NewIntensity = FadeAlpha * (LightIntensity - EmbersIntensity) + EmbersIntensity;

	// update the glow light's intensity
	LightComponent->SetIntensity(NewIntensity);
}