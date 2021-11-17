// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxUnitManagerComponent.h"
#include "UnitComponent.h"
#include "BeaconLog.h"

UBoxUnitManagerComponent::UBoxUnitManagerComponent()
{
}

// Called every frame
void UBoxUnitManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TQueue<UUnitComponent*> temp;

	UUnitComponent* unit;
	
	while(m_TriggeredUnits.Dequeue(unit))
	{
		bool flag = false;
		if (unit->IsTriggered())
		{
			*unit += 10.f;
		}
		for (auto neighbor : unit->GetNeighbors()->neighbors)
		{
			if (neighbor != nullptr && !neighbor->IsTriggered())
			{
				if (*unit > *neighbor)
				{
					//TODO: use simulation function
					*unit -= 0.01;
					*neighbor += 0.01;
					if (*neighbor > 5.f)
					{
						neighbor->Trigger(m_Particle);
						temp.Enqueue(unit);
					}
				}
				if (!flag && *neighbor < 5.f)
				{
					temp.Enqueue(unit);
					flag = true;
				}
			}
		}
	}

	while (temp.Dequeue(unit))
	{
		m_TriggeredUnits.Enqueue(unit);
	}
}

void UBoxUnitManagerComponent::OnUnregister()
{
	for (auto unit : m_Units)
	{
		unit->UnregisterComponent();
	}
	m_TriggeredUnits.Empty();
	m_Units.Empty();

	Super::OnUnregister();
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