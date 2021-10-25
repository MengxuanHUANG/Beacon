// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BeaconConnection.h"

class UBoxComponent;

/**
 * 
 */
class BEACON_API Unit
{
public:
	Unit() {}
	virtual ~Unit() {};

	virtual void Initialize(FVector extent, ConnectType type) = 0;
	virtual void Ignite(UParticleSystem* particle) = 0;
	virtual void SetNeighbor(int x, int y, int z, Unit* unit) = 0;
	virtual const TArray<Unit*>& GetNeighbors() const = 0;

	/*
	* Functions for debugging
	*/
	virtual void IGetName(FString& name) = 0;
	virtual void DisplayDebugInfo() {}
	virtual void OnDestroy(bool bPromoteChildren) = 0;
public:
	

	UPROPERTY(EditAnyWhere)
		ConnectType m_ConnectType;
};
