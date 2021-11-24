// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereUnitManagerComponent.h"
#include "UnitComponent.h"
#include "BeaconLog.h"

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

	TQueue<UUnitComponent*> temp;
	int32 count = 0;

	UUnitComponent* unit;
	while (m_TriggeredUnits.Dequeue(unit))
	{
		count++;

		bool flag = false;
		if (unit->IsTriggered())
		{
			*unit += 5.f;
		}
		for (auto neighbor : unit->GetNeighbors()->neighbors)
		{
			if (neighbor != nullptr && !neighbor->IsTriggered())
			{
				if (*unit > *neighbor)
				{
					//TODO: use simulation function
					*unit -= 0.01;
					*neighbor += 0.02;
					if (*neighbor > 2.f)
					{
							neighbor->Trigger(m_Particle);
							temp.Enqueue(neighbor);

					}
				}
				if (!flag && *neighbor < 2.f)
				{
					temp.Enqueue(unit);
					flag = true;
				}
			}
		}
	}

	BEACON_LOG(Warning, "%d units need to be updated", count);
	while (temp.Dequeue(unit))
	{
		m_TriggeredUnits.Enqueue(unit);
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