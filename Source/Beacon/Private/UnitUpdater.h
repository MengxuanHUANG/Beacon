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
	UnitUpdater();
	virtual ~UnitUpdater();

	//calculate new value for the unit and the queue
	virtual void UpdateUnit(float deltaTime, Beacon_PriorityQueue<UUnitComponent>& updateList, TSubclassOf<UBeaconFire>& beaconFire, uint32 unitsCount) const;

	//Update Units
	virtual void LateUpdateUnit(float deltaTime, Beacon_PriorityQueue<UUnitComponent>& updateList, uint32 unitsCount) const;

};
