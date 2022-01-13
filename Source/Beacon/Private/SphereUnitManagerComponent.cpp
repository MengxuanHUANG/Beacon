// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereUnitManagerComponent.h"
#include "UnitComponent.h"
#include "BeaconLog.h"
#include "UnitUpdater.h"

USphereUnitManagerComponent::USphereUnitManagerComponent()
{
}

USphereUnitManagerComponent::~USphereUnitManagerComponent()
{
}

// Called every frame
void USphereUnitManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (m_UnitUpdater.IsValid())
	{
		//m_UnitUpdater->UpdateUnit(m_TriggeredUnits, m_UnitCount.X * m_UnitCount.Y * m_UnitCount.Z);
	}
}

void USphereUnitManagerComponent::OnUnregister()
{
	for (auto unit : m_Units)
	{
		unit.Value->UnregisterComponent();
	}
	m_TriggeredUnits.Empty();
	m_Units.Empty();

	Super::OnUnregister();
}

void USphereUnitManagerComponent::UpdateUnits()
{
}

void USphereUnitManagerComponent::SetParameter(uint32 x)
{
	m_Count = x;
}

UUnitComponent* USphereUnitManagerComponent::GetUnit(FVector index)
{
	if (!m_Units.Contains(index))
	{
		return nullptr;
	}
	else
	{
		return m_Units[index];
	}
}

void USphereUnitManagerComponent::TriggerUnit_Implementation(FVector index, float initValue)
{
}

void USphereUnitManagerComponent::SetBeaconFire(TSubclassOf<UBeaconFire>& beaconFire)
{
	m_BeaconFire = beaconFire;
}