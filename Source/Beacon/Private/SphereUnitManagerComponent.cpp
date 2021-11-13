// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereUnitManagerComponent.h"
#include "UnitComponent.h"

USphereUnitManagerComponent::USphereUnitManagerComponent()
{}

USphereUnitManagerComponent::~USphereUnitManagerComponent()
{
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
	return nullptr;
}

void USphereUnitManagerComponent::TriggerUnit(FVector index)
{
	UUnitComponent* unit;
	if ((unit = GetUnit(index)) != nullptr)
	{
		unit->Trigger(m_Particle);
		m_TriggeredUnits.Enqueue(unit);
	}
}

void USphereUnitManagerComponent::SetParticle(UParticleSystem* particle)
{
	m_Particle = particle;
}