// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// lerp between positions based on duration time
	Position = IsOpen ? FMath::Clamp(Position + (DeltaTime / Duration), 0, 1) : FMath::Clamp(Position - (DeltaTime / Duration), 0, 1);
	SetActorLocation(FMath::Lerp(ClosedLocation, OpenLocation, Position));
}

void ADoor::Activate() { Open(); }
void ADoor::Deactivate() { Close(); }

void ADoor::Open() 
{
	IsOpen = true;
}

void ADoor::Close() 
{
	IsOpen = false;
}

