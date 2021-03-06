// Fill out your copyright notice in the Description page of Project Settings.


#include "BeaconFire.h"

// Sets default values for this component's properties
UBeaconFire::UBeaconFire()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UBeaconFire::StartBurning_Implementation()
{
}

void UBeaconFire::EndFlame_Implementation()
{
}

void UBeaconFire::EndBurning_Implementation()
{
}

void UBeaconFire::CallBurningEvent(FString eventName)
{
	F_BurningEvents.Broadcast(eventName);
}