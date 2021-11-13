// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereUnitManagerComponent.h"

USphereUnitManagerComponent::USphereUnitManagerComponent()
{}

USphereUnitManagerComponent::~USphereUnitManagerComponent()
{
}

void USphereUnitManagerComponent::UpdateUnits()
{
}

void USphereUnitManagerComponent::SetParameter(uint32 x)
{
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