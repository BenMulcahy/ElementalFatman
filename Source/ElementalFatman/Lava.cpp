// Fill out your copyright notice in the Description page of Project Settings.


#include "Lava.h"

ALava::ALava() 
{
	GlowLight = CreateDefaultSubobject<URectLightComponent>(TEXT("Rect Light"));
	GlowLight->SetupAttachment(Mesh);

	GlowLight->SetRelativeLocationAndRotation(FVector(0, 0, 250), FRotator(-90, 0, 0));
	GlowLight->Intensity = 1250;
	GlowLight->LightColor = FColor::White;
	GlowLight->AttenuationRadius = 1000;
	GlowLight->SourceWidth = 100;
	GlowLight->SourceHeight = 200;
	GlowLight->BarnDoorLength = 0;
	GlowLight->bUseTemperature = true;
	GlowLight->Temperature = 1700;
}

void ALava::Setup()
{
	if (!OverrideMesh)
	{
		// construct mesh here
	}

	ObjectType = EObjectType::OT_Lava;

	UE_LOG(LogInteraction, Warning, TEXT("setting up lava"));

	MaxInteractablePips = 1;
	CurrentInteractablePips = 1;

	// set initial lava collision state
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	GlowIntensity = GlowLight->Intensity;

	Super::Setup();
}


void ALava::InvokeSpecificMechanic()
{
	FTimerDelegate FadeDelegate;

	switch (CurrentInteractablePips)
	{
	case 0: // cooled lava
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("solidified lava")));

		// update the mesh collision (can't walk into cooled lava)
		Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

		// fade out the lava glow effect
		FadeDelegate.BindUFunction(this, "FadeIntensity", false);
		FadeAlpha = 1;
		GetWorld()->GetTimerManager().SetTimer(FadeHandle, FadeDelegate, GetWorld()->DeltaTimeSeconds, true);

		break;
	case 1: // heated lava
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("liquified lava")));

		// update the mesh collision (can walk into hot lava & die)
		Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

		// fade in the lava glow effect
		FadeDelegate.BindUFunction(this, "FadeIntensity", true);
		FadeAlpha = 0;
		GetWorld()->GetTimerManager().SetTimer(FadeHandle, FadeDelegate, GetWorld()->DeltaTimeSeconds, true);

		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Lava current pip value error!"));
		break;
	}
}

void ALava::FadeIntensity(bool increasing)
{
	// finish increasing/decreasing the glow once the alpha reaches its desired end value
	if (increasing ? FadeAlpha >= 1 : FadeAlpha <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(FadeHandle);
		GlowLight->SetIntensity(increasing ? GlowIntensity : 0); // ensure the end value is exact
		return;
	}

	else
	{
		// change the alpha value linearly between 0 and 1 (or 1 and 0 if fading out)
		FadeAlpha += GetWorld()->GetTime().GetDeltaWorldTimeSeconds() / (increasing ? GlowFadeDuration : -GlowFadeDuration);

		// update the glow light's intensity
		GlowLight->SetIntensity(FadeAlpha * GlowIntensity);
	}
}