// Fill out your copyright notice in the Description page of Project Settings.


#include "CapsuleUnitManagerComponent.h"
#include "UnitComponent.h"

UCapsuleUnitManagerComponent::UCapsuleUnitManagerComponent()
{
}

UCapsuleUnitManagerComponent::~UCapsuleUnitManagerComponent()
{
}

void UCapsuleUnitManagerComponent::OnUnregister()
{
	for (auto unit : m_Units)
	{
		unit.Value->UnregisterComponent();
	}
	m_Units.Empty();

	Super::OnUnregister();
}

void UCapsuleUnitManagerComponent::UpdateUnits()
{
}

UUnitComponent* UCapsuleUnitManagerComponent::GetUnit(FVector index)
{
	return nullptr;
}

void UCapsuleUnitManagerComponent::TriggerUnit(FVector index)
{
}

void UCapsuleUnitManagerComponent::SetParameter(uint32 x)
{
	m_Count = x;
}

void UCapsuleUnitManagerComponent::SetParameter2(uint32 x, uint32 y)
{
	m_Count = x;
	m_Count_height = y;
}

void UCapsuleUnitManagerComponent::SetParameter3(uint32 x, uint32 y, uint32 z)
{
}

void UCapsuleUnitManagerComponent::SetBeaconFire(TSubclassOf<UBeaconFire>& beaconFire)
{
}