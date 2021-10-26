// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BeaconConnection.h"

class USceneComponent;
class Unit;

class BEACON_API UnitsManager
{
public:
	UnitsManager(ConnectType type)
		:m_ConnectType(type)
	{}
	virtual ~UnitsManager() {}

	virtual void CreateUnits(USceneComponent* self, const USceneComponent* parent) = 0;
	virtual Unit* GetUnit(FVector index) = 0;
	virtual void UpdateUnits() = 0;
	virtual void Destroy(bool bPromoteChildren) = 0;
public:
	ConnectType m_ConnectType;

public:
	template<typename T>
	static UnitsManager* CreateUnitsManager(ConnectType type, uint32 x, uint32 y, uint32 z)
	{
		return new T(type, x, y, z);
	}

	template<typename T>
	static UnitsManager* CreateUnitsManager(ConnectType type, uint32 count)
	{
		return new T(type, count);
	}
};