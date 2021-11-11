// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BeaconConnection.h"
#include "UObject/NoExportTypes.h"
#include "UnitManager.generated.h"

/**
 * 
 */

class USceneComponent;
class UUnitComponent;

UCLASS()
class BEACON_API UUnitManager : public UObject
{
	GENERATED_BODY()

public:
	UUnitManager() {}

	virtual ~UUnitManager() {}
	
	virtual void CreateUnits(USceneComponent* self, const USceneComponent* parent) {}
	
	virtual void TriggerUnit(FVector index) {}
	virtual UUnitComponent* GetUnit(FVector index) { return nullptr; }
	virtual void UpdateUnits() {}

	virtual void SetParticle(UParticleSystem* particle) {}

	virtual void SetParameter(uint32 x) {}
	virtual void SetParameter3(uint32 x, uint32 y, uint32 z);

public:
	inline void SetConnectType(ConnectType type) { m_ConnectType = type; }
	inline ConnectType GetConnectType() const { return m_ConnectType; }

protected:
	ConnectType m_ConnectType;
};
