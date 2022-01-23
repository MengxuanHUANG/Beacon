// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Beacon_PriorityQueue.h"

class UBeaconMaterial;
class UBeaconFire;

/**
 * 
 */
class UnitUpdater
{
public:
	UnitUpdater(const UBeaconMaterial* material);
	virtual ~UnitUpdater();

	//calculate new value for the unit and the queue
	virtual void UpdateUnit(float deltaTime, Beacon_PriorityQueue<UUnitComponent>& updateList, TSubclassOf<UBeaconFire>& beaconFire, uint32 unitsCount) const;

public:
	//set a new material for this updater
	inline void SetMaterial(const UBeaconMaterial* material)
	{
		m_Material = material;
	}

	inline const UBeaconMaterial* GetMaterial() const
	{
		return m_Material;
	}

protected:
	const UBeaconMaterial* m_Material;
};
