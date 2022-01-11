// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitUpdater.h"
#include "UnitComponent.h"
#include "BeaconMaterial.h"

#include "BeaconLog.h"

UnitUpdater::UnitUpdater(const UBeaconMaterial* material)
	:m_Material(material)
{

}

UnitUpdater::~UnitUpdater()
{
	m_Material = nullptr;
}

void UnitUpdater::UpdateUnit(Beacon_PriorityQueue<UUnitComponent>& triggeredUnits, uint32 unitsCount) const
{
	BEACON_LOG(Display, "UpdateUnit in UnitUpdater");
	//TODO: calculate new value for the unit and the queue

	/*
	TQueue<UUnitComponent*> temp;
	bool* arr = new bool[m_UnitCount.X * m_UnitCount.Y * m_UnitCount.Z] {false};
	int32 count = 0;

	UUnitComponent* unit;
	while(m_TriggeredUnits.Dequeue(unit))
	{
		count++;

		bool flag = false;

		*unit += 10.f; //TODO: remove

		//calculate cumulated thermal
		for (auto neighbor : unit->GetNeighbors()->neighbors)
		{

		}

		for (auto neighbor : unit->GetNeighbors()->neighbors)
		{
			if (neighbor != nullptr && !neighbor->IsTriggered())
			{
				if (m_Material->GetTemperature(unit->GetValue()) > m_Material->GetTemperature(neighbor->GetValue()))
				{
					//TODO: use simulation function
					*unit -= 1;
					*neighbor += 1;
					float neighborTemperature = m_Material->GetTemperature(neighbor->GetValue());
					if (neighborTemperature > m_Material->Flash_Point)
					{
						if (!arr[neighbor->GetIndex()])
						{
							neighbor->Trigger(m_Particle);
							temp.Enqueue(neighbor);
							arr[neighbor->GetIndex()] = true;
						}
					}
				}
				if (!flag && m_Material->GetTemperature(neighbor->GetValue()) < m_Material->Flash_Point)
				{
					temp.Enqueue(unit);
					flag = true;
				}
			}
		}
	}

	delete[] arr;

	BEACON_LOG(Warning, "%d units need to be updated", count);
	while (temp.Dequeue(unit))
	{
		m_TriggeredUnits.Enqueue(unit);
	}
	*/
}