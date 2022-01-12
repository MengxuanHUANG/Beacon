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

void UnitUpdater::UpdateUnit(float deltaTime, Beacon_PriorityQueue<UUnitComponent>& triggeredUnits, TSubclassOf<UBeaconFire>& beaconFire, uint32 unitsCount) const
{
	//TODO: calculate new value for the unit and the queue

	TQueue<UUnitComponent*> temp;
	bool* arr = new bool[unitsCount] {false};

	BEACON_LOG(Warning, "%d units need to be updated", triggeredUnits.Num());

	UUnitComponent* unit;
	while(!triggeredUnits.IsEmpty())
	{
		unit = triggeredUnits.Pop();

		//update unit itself
		if (unit->Update(deltaTime))
		{
			arr[unit->GetIndex()] = true;
			temp.Enqueue(unit);
		}

		//traverse all neighbors of a unit 
		for (auto neighbor : unit->GetNeighbors()->neighbors)
		{
			//ignore thermal exchange among burning units
			if (neighbor != nullptr)
			{
				if (!neighbor->IsTriggered())
				{
					float gap = unit->GetTemperature() - neighbor->GetTemperature();
					unit->Value -= deltaTime;
					neighbor->Value += deltaTime;

					if (unit->Value > m_Material->Flash_Point)
					{
						if (!arr[neighbor->GetIndex()])
						{
							neighbor->Trigger(beaconFire);
							temp.Enqueue(neighbor);
							arr[neighbor->GetIndex()] = true;
						}
					}
				}
			}
			else
			{
				float loss = deltaTime * (m_Material->LoseThermalPerSecond / float(unit->GetNeighbors()->neighbors.Num()));
				unit->Value -= loss;
			}
		}
	}

	delete[] arr;
	
	while (temp.Dequeue(unit))
	{
		triggeredUnits.Push(unit);
	}
}