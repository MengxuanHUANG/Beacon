// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxUnitManagerComponent.h"
#include "UnitComponent.h"
#include "BeaconLog.h"

UBoxUnitManagerComponent::UBoxUnitManagerComponent()
{
}

UBoxUnitManagerComponent::~UBoxUnitManagerComponent()
{
}

void UBoxUnitManagerComponent::UpdateUnits()
{
}

UUnitComponent* UBoxUnitManagerComponent::GetUnit(FVector index)
{
	if (index.X * m_UnitCount.Y * m_UnitCount.Z + index.Y * m_UnitCount.Z + index.Z >= m_Units.Num())
	{
		return nullptr;
	}
	else
	{
		return m_Units[index.X * m_UnitCount.Y * m_UnitCount.Z + index.Y * m_UnitCount.Z + index.Z];
	}
}

void UBoxUnitManagerComponent::TriggerUnit(FVector index)
{
	UUnitComponent* unit;
	if ((unit = GetUnit(index)) != nullptr)
	{
		unit->Trigger(m_Particle);
		m_TriggeredUnits.Enqueue(unit);
	}
}

void UBoxUnitManagerComponent::SetParameter3(uint32 x, uint32 y, uint32 z)
{
	m_UnitCount.X = x;
	m_UnitCount.Y = y;
	m_UnitCount.Z = z;
}

void UBoxUnitManagerComponent::SetParticle(UParticleSystem* particle)
{
	m_Particle = particle;
}