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

	if (opening) SetActorLocation(GetActorLocation() + FVector(0, 5, 0));
}

void ADoor::Open() 
{
	StartOpening();
	GetWorld()->GetTimerManager().SetTimer(OpenHandler, this, &ADoor::StopOpening, 1.5f, false);
}

void ADoor::StartOpening() 
{
	opening = true;
}

void ADoor::StopOpening()
{
	opening = false;
}

