// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitComponent.h"
#include "BeaconMaterial.h"

// Sets default values for this component's properties
UUnitComponent::UUnitComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

// Called when the game starts
void UUnitComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

float UUnitComponent::GetTemperature() const
{
	return m_Material->GetTemperature(Value);
}

bool UUnitComponent::operator < (const UUnitComponent& unit)
{
	return this->Value < unit.Value;
}

bool UUnitComponent::operator <= (const UUnitComponent& unit) 
{
	return this->Value <= unit.Value;
}

bool UUnitComponent::operator > (const UUnitComponent& unit) 
{
	return this->Value > unit.Value;
}

bool UUnitComponent::operator >= (const UUnitComponent& unit) 
{
	return this->Value >= unit.Value;
}

bool UUnitComponent::operator == (const UUnitComponent& unit) 
{
	return this->Value == unit.Value;
}

bool UUnitComponent::operator != (const UUnitComponent& unit)
{
	return !(*this == unit);
}

bool UUnitComponent::operator < (float value)
{
	return this->Value < value;
}

bool UUnitComponent::operator <= (float value)
{
	return this->Value <= value;
}

bool UUnitComponent::operator > (float value)
{
	return this->Value > value;
}

bool UUnitComponent::operator >= (float value)
{
	return this->Value >= value;
}

bool UUnitComponent::operator == (float value)
{
	return this->Value == value;
}

bool UUnitComponent::operator != (float value)
{
	return !(this->Value == value);
}