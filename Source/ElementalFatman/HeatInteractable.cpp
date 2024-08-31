// Fill out your copyright notice in the Description page of Project Settings.


#include "HeatInteractable.h"

// Sets default values
AHeatInteractable::AHeatInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHeatInteractable::BeginPlay()
{
	Super::BeginPlay();
	SetupInstancedMaterial();
}

void AHeatInteractable::SetupInstancedMaterial()
{
	Mesh = CastChecked<UMeshComponent>(GetComponentByClass<UMeshComponent>());
	if (Mesh) { UE_LOG(LogTemp, Warning, TEXT("Found mesh: %s"), *Mesh->GetName()); }
	else UE_LOG(LogTemp, Warning, TEXT("No mesh found"));

	DynamicMat = UMaterialInstanceDynamic::Create(Mesh->GetMaterials()[0], this);
}

void AHeatInteractable::ChangeColor(FColor color)
{
	DynamicMat->SetVectorParameterValue("TestColour", color);
	Mesh->SetMaterial(0, DynamicMat);
}


// Called every frame
void AHeatInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ChangeColor(FColor::Blue);
}

