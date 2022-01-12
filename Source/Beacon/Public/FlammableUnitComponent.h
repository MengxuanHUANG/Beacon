// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitComponent.h"
#include "BeaconFire.h"
#include "BeaconCore.h"
#include "FlammableUnitComponent.generated.h"


/**
 * 
 */
UCLASS()
class BEACON_API UFlammableUnitComponent : public UUnitComponent
{
	GENERATED_BODY()
public:
	UFlammableUnitComponent();
	
	// Called when the game starts
	virtual void BeginPlay() override;
	
	virtual void OnUnregister() override;

public:
	virtual bool Update(float deltaTime);
	virtual void Initialize(FVector extent, ConnectType type);
	virtual void Trigger(TSubclassOf<UBeaconFire>& beaconFire);
	virtual void SetNeighbor(int x, int y, int z, UUnitComponent* unit);
	virtual UNeighbor* GetNeighbors() const { return m_Neighbors; }

	virtual void DisplayDebugInfo();

public:
	virtual bool IsTriggered() const override { return b_IsBurning; }

public:
	UPROPERTY(VisibleAnyWhere)
		UBeaconFire* m_BeaconFire;

	UPROPERTY(VisibleAnyWhere)
		FVector m_UnitExtent;
private:
	UPROPERTY(VisibleAnyWhere)
		bool b_IsBurning;

	UPROPERTY(VisibleAnyWhere)
		float m_TotalBurningTime;

	int32 m_BurningEventCount;
};
