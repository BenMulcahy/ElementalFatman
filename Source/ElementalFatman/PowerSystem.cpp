// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerSystem.h"

// Sets default values
APowerSystem::APowerSystem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

// Called when the game starts or when spawned
void APowerSystem::BeginPlay()
{
	Super::BeginPlay();

	if (!AreEntriesValid()) UE_LOG(LogTemp, Error, TEXT("Invalid power system!!! Need at least one power supplier and one receiver."));
	SetupPowerSuppliers(); 
	SetupPowerFreezers();
}

// horrible stuff in tick, will be replaced when wires are added in future
void APowerSystem::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (ShowDebugLine) 
	{
		bool PoweredOn = false;
		for (int i = 0; i < PowerSuppliers.Num(); i++)
		{
			PoweredOn = CurrentPowerStates[i] == RequiredPowerStates[i] ? true : false;

			for (int j = 0; j < PowerReceivers.Num(); j++)
			{
				DrawDebugLine(GetWorld(), PowerSuppliers[i]->PowerSupply->GetActorLocation(), PowerReceivers[j]->GetActorLocation(), PoweredOn ? FColor::Green : FColor::Red, false);
			}
		}

		bool PowerFrozen = false;
		for (int i = 0; i < PowerFreezers.Num(); i++)
		{
			PowerFrozen = CurrentFreezerStates[i] == RequiredFreezerStates[i] ? true : false;

			for (int j = 0; j < PowerReceivers.Num(); j++)
			{
				DrawDebugLine(GetWorld(), PowerFreezers[i]->PowerSupply->GetActorLocation(), PowerReceivers[j]->GetActorLocation(), PowerFrozen ? FColor::Green : FColor::Red, false);
			}
		}
	}
}

UPowerSupplierInstance::UPowerSupplierInstance() 
{
	PowerSupply = nullptr;
	TypeOfSupply = ESupplyType::ST_Default;
	GeneratorMustBe = EGeneratorState::Off;
	FanMustBe = EFanState::Clockwise;
	PressurePlateMustBe = EPressurePlateState::Released;
	MovingMechanismMustBe = EMovingMechanismState::Off;
	ClockMustBe = EClockState::Complete;
	SwitchMustBe = ESwitchState::Neutral;
}

#if WITH_EDITOR
void UPowerSupplierInstance::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (Cast<AGenerator>(PowerSupply)) TypeOfSupply = ESupplyType::ST_Generator;
	if (Cast<AFan>(PowerSupply)) TypeOfSupply = ESupplyType::ST_Fan;
	if (Cast<AMovingMechanism>(PowerSupply)) TypeOfSupply = ESupplyType::ST_Moving;
	if (Cast<APressurePlate>(PowerSupply)) TypeOfSupply = ESupplyType::ST_Pressure;
	if (Cast<AClock>(PowerSupply)) TypeOfSupply = ESupplyType::ST_Clock;
	if (Cast<AThreeWaySwitch>(PowerSupply)) TypeOfSupply = ESupplyType::ST_Switch;
	// add more power supply types here
}
#endif

bool APowerSystem::AreEntriesValid() 
{
	// power system is only valid if there is:
	// at least one supplier
	if (PowerSuppliers.Num() == 0) return false;
	// at least one receiver
	if (PowerReceivers.Num() == 0) return false;
	return true;
}

void APowerSystem::SetupPowerSuppliers() 
{
	// loop through all power suppliers
	for (int i = 0; i < PowerSuppliers.Num(); i++)
	{
		// make a list of each power supply's current heat/cool state
		CurrentPowerStates.Add(PowerSuppliers[i]->PowerSupply->GetCurrentInteractablePips());

		// subscribe to each power supply's delegate, which broadcasts whenever the power supply is heated or cooled and returns its pip value
		PowerSuppliers[i]->PowerSupply->PowerStateChangedDelegate.AddUniqueDynamic(this, &APowerSystem::UpdatePowerState);
		FString s = "";
		// create an array of desired power states, taking values from different "must be" variables depending on the "type of supply" variable
		switch (PowerSuppliers[i]->TypeOfSupply)
		{
		case ESupplyType::ST_Default:
			UE_LOG(LogTemp, Warning, TEXT("invalid supply type"));
			break;
		case ESupplyType::ST_Generator:
			RequiredPowerStates.Add((int)PowerSuppliers[i]->GeneratorMustBe);
			break;
		case ESupplyType::ST_Fan:
			RequiredPowerStates.Add((int)PowerSuppliers[i]->FanMustBe);
			s = PowerSuppliers[i]->FanMustBe == EFanState::Clockwise ? "clockwise" : "anticlockwise";
			UE_LOG(LogTemp, Warning, TEXT("%s, %d"), *s, (int)PowerSuppliers[i]->FanMustBe);
			break;		
		case ESupplyType::ST_Pressure:
			RequiredPowerStates.Add((int)PowerSuppliers[i]->PressurePlateMustBe);
			break;		
		case ESupplyType::ST_Moving:
			RequiredPowerStates.Add((int)PowerSuppliers[i]->MovingMechanismMustBe);
			break;
		case ESupplyType::ST_Clock:
			RequiredPowerStates.Add((int)PowerSuppliers[i]->ClockMustBe);
			break;
		case ESupplyType::ST_Switch:
			RequiredPowerStates.Add((int)PowerSuppliers[i]->SwitchMustBe);
			break;

		// add more power supply types here
		default:
			break;
		}
	}
}

void APowerSystem::SetupPowerFreezers()
{
	// loop through all power suppliers
	for (int i = 0; i < PowerFreezers.Num(); i++)
	{
		// make a list of each power supply's current heat/cool state
		CurrentFreezerStates.Add(PowerFreezers[i]->PowerSupply->GetCurrentInteractablePips());

		// subscribe to each power supply's delegate, which broadcasts whenever the power supply is heated or cooled and returns its pip value
		PowerFreezers[i]->PowerSupply->PowerStateChangedDelegate.AddUniqueDynamic(this, &APowerSystem::UpdatePowerState);

		// create an array of desired power states, taking values from different "must be" variables depending on the "type of supply" variable
		switch (PowerFreezers[i]->TypeOfSupply)
		{
		case ESupplyType::ST_Default:
			UE_LOG(LogTemp, Warning, TEXT("invalid freezer type"));
			break;
		case ESupplyType::ST_Generator:
			RequiredFreezerStates.Add((int)PowerFreezers[i]->GeneratorMustBe);
			break;
		case ESupplyType::ST_Fan:
			RequiredFreezerStates.Add((int)PowerFreezers[i]->FanMustBe);
			break;
		case ESupplyType::ST_Pressure:
			RequiredFreezerStates.Add((int)PowerFreezers[i]->PressurePlateMustBe);
			break;
		case ESupplyType::ST_Moving:
			RequiredFreezerStates.Add((int)PowerFreezers[i]->MovingMechanismMustBe);
			break;
		case ESupplyType::ST_Clock:
			RequiredFreezerStates.Add((int)PowerFreezers[i]->ClockMustBe);
			break;
		case ESupplyType::ST_Switch:
			RequiredFreezerStates.Add((int)PowerFreezers[i]->SwitchMustBe);
			break;

		// add more power freezer types here
		default:
			break;
		}
	}
}

void APowerSystem::UpdatePowerState(APowerSupply* UpdatedPowerSupply, int32 NewPowerState)
{	
	UE_LOG(LogTemp, Warning, TEXT("called by %s, %d pips"), *UpdatedPowerSupply->GetName(), NewPowerState);

	for (int i = 0; i < PowerSuppliers.Num(); i++) 
	{
		if (PowerSuppliers[i]->PowerSupply == UpdatedPowerSupply)
		{
			UE_LOG(LogTemp, Warning, TEXT("required pips: %d"), RequiredPowerStates[i])
			//UE_LOG(LogTemp, Warning, TEXT("found correct updated supply"));
			CurrentPowerStates[i] = NewPowerState;
		}
	}

	for (int i = 0; i < PowerFreezers.Num(); i++)
	{
		if (PowerFreezers[i]->PowerSupply == UpdatedPowerSupply) 
		{
			CurrentFreezerStates[i] = NewPowerState;
		}
	}

	// check if the power system is frozen; if not, check if the power system is now powered/unpowered
	if (!IsPowerFrozen()) for (APowerReceiver* PowerReceiver : PowerReceivers) PowerReceiver->Power(IsPowerSupplied());
}

bool APowerSystem::IsPowerFrozen() 
{
	// if there aren't any power freezers, power can't be frozen
	if (RequiredFreezerStates.Num() == 0) return false;

	int CorrectFreezerStates = 0;
	for (int i = 0; i < CurrentFreezerStates.Num(); i++)
	{
		if (CurrentFreezerStates[i] == RequiredFreezerStates[i]) CorrectFreezerStates++;
	}

	if (CorrectFreezerStates == RequiredFreezerStates.Num()) return true;
	else return false;
}

bool APowerSystem::IsPowerSupplied()
{
	int CorrectPowerStates = 0;
	for (int i = 0; i < CurrentPowerStates.Num(); i++)
	{
		if (CurrentPowerStates[i] == RequiredPowerStates[i]) CorrectPowerStates++;
	}

	if (CorrectPowerStates == RequiredPowerStates.Num()) return true;
	else return false;
}
