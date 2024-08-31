// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ElementalFatmanGameMode.generated.h"

UCLASS(minimalapi)
class AElementalFatmanGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AElementalFatmanGameMode();
	TArray<AActor*> GetSceneInteractables() const { return SceneInteractables; }
	void SetSceneInteractables(TArray<AActor*> interactables) { SceneInteractables = interactables; }

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> FindSceneInteractables();

protected:
	TArray<AActor*> SceneInteractables;

};



