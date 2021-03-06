// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitUpdater.h"
#include "UnitComponent.h"
#include "BeaconMaterial.h"
#include "UnitManagerComponent.h"

#include "BeaconLog.h"

UnitUpdater::UnitUpdater()
{
}

UnitUpdater::~UnitUpdater()
{
}

void UnitUpdater::UpdateUnit(float deltaTime, Beacon_PriorityQueue<UUnitComponent>& updateList, TSubclassOf<UBeaconFire>& beaconFire, uint32 unitsCount) const
{
	TQueue<UUnitComponent*> tempList;
	int size = unitsCount;
	bool* isInList = new bool[size];
	for (int i = 0; i < size; i++) { isInList[i] = false; }

	//BEACON_LOG(Warning, "%d units need to be updated", triggeredUnits.Num());

	UUnitComponent* unit;
	while(!updateList.IsEmpty())
	{
		unit = updateList.Pop();
		tempList.Enqueue(unit);

		//Exchange thermal energy only if unit is triggered
		if (unit->IsTriggered())
		{
			float unitTemp = unit->GetTemperature();
			float thermalExchangeCoefficient = unit->GetMaterial()->Thermal_Exchange_Coefficient;
			//traverse all neighbors of a unit 
			for (auto neighbor : unit->GetNeighbors()->neighbors)
			{
				//ignore thermal exchange among burning units
				if (neighbor != nullptr && !neighbor->IsTriggered())
				{
					float gap = unitTemp - neighbor->GetTemperature();
					float deltaThermal = thermalExchangeCoefficient * deltaTime * gap / unit->Value;

					neighbor->Value += deltaThermal;

					if (!isInList[neighbor->GetIndex()])
					{
						isInList[neighbor->GetIndex()] = true;
						neighbor->SetFlag(EUnitFlag::NeedUpdate);
						tempList.Enqueue(neighbor);
					}
				}
			}

			//traverse all temp neighbors of a unit 
			TArray<TSharedPtr<UnitConnection>> tempConnections;
			unit->GetTemporaryNeighbors(tempConnections);
			for (TSharedPtr<UnitConnection>& connection : tempConnections)
			{
				if (!connection->Other->IsTriggered())
				{
					float gap = unitTemp - connection->Other->GetTemperature();
					float deltaThermal = thermalExchangeCoefficient * deltaTime * gap / unit->Value;

					connection->Other->Value += deltaThermal;
				}
			}
		}
	}

	unit = nullptr;
	while (tempList.Dequeue(unit))
	{
		updateList.Push(unit);
	}

	delete[] isInList;
}

void UnitUpdater::LateUpdateUnit(float deltaTime, Beacon_PriorityQueue<UUnitComponent>& updateList, uint32 unitsCount) const
{
	TQueue<UUnitComponent*> tempList;

	UUnitComponent* unit;
	while (!updateList.IsEmpty())
	{
		unit = updateList.Pop();

		//update unit itself
		unit->Update(deltaTime);

		if (unit->CheckFlag(EUnitFlag::NeedUpdate))
		{
			tempList.Enqueue(unit);
		}
	}

	unit = nullptr;
	while (tempList.Dequeue(unit))
	{
		updateList.Push(unit);
	}
}