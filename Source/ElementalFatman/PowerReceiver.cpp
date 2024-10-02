// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerReceiver.h"

void APowerReceiver::Power(bool ReceivingPower) 
{
	IsPowered = ReceivingPower;
	IsPowered ? Activate() : Deactivate();
}

void APowerReceiver::Activate() {}
void APowerReceiver::Deactivate() {}