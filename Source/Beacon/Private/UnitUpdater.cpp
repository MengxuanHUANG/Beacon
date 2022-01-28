// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitUpdater.h"
#include "UnitComponent.h"
#include "BeaconMaterial.h"
#include "UnitManagerComponent.h"

#include "BeaconLog.h"

UnitUpdater::UnitUpdater(const UBeaconMaterial* material)
	:m_Material(material)
{

}

UnitUpdater::~UnitUpdater()
{
	m_Material = nullptr;
}

void UnitUpdater::UpdateUnit(float deltaTime, Beacon_PriorityQueue<UUnitComponent>& updateList, TSubclassOf<UBeaconFire>& beaconFire, uint32 unitsCount) const
{
	TQueue<UUnitComponent*> tempList;
	TQueue<UUnitComponent*> checkList;
	int size = unitsCount;
	bool* isInList = new bool[size];
	for (int i = 0; i < size; i++) { isInList[i] = false; }

	//BEACON_LOG(Warning, "%d units need to be updated", triggeredUnits.Num());

	UUnitComponent* unit;
	while(!updateList.IsEmpty())
	{
		unit = updateList.Pop();

		if (unit->CheckFlag(UnitFlag::NeedUpdate))
		{
			//update unit itself
			unit->Update(deltaTime);
		}

		//Exchange thermal energy only if unit is triggered
		if (unit->CheckFlag(UnitFlag::Triggered))
		{
			//avoid repeating add same unit to queue
			isInList[unit->GetIndex()] = true;
			tempList.Enqueue(unit);

			//traverse all neighbors of a unit 
			for (auto neighbor : unit->GetNeighbors()->neighbors)
			{
				//ignore thermal exchange among burning units
				if (neighbor != nullptr && !neighbor->CheckFlag(UnitFlag::Triggered))
				{
					float gap = unit->GetTemperature() - neighbor->GetTemperature();
					unit->Value -= deltaTime;
					neighbor->Value += deltaTime;

					if (!isInList[neighbor->GetIndex()])
					{
						isInList[neighbor->GetIndex()] = true;
						checkList.Enqueue(neighbor);
					}
				}
			}

			//traverse all temp neighbors of a unit 
			TArray<TSharedPtr<UnitConnection>> tempConnections;
			unit->GetTemporaryNeighbors(tempConnections);
			for (TSharedPtr<UnitConnection>& connection : tempConnections)
			{
				if (!connection->Other->CheckFlag(UnitFlag::Triggered))
				{
					connection->Other->Value += deltaTime;
				}
			}
		}
	}

	while (checkList.Dequeue(unit))
	{
		if (unit->Value > m_Material->Flash_Point)
		{
			unit->Trigger(beaconFire);
			tempList.Enqueue(unit);
		}
	}

	delete[] isInList;
	
	while (tempList.Dequeue(unit))
	{
		updateList.Push(unit);
	}
}