// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitComponent.h"
#include "NonflammableUnitComponent.generated.h"

/**
 * 
 */
UCLASS()
class BEACON_API UNonflammableUnitComponent : public UUnitComponent
{
	GENERATED_BODY()
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void Initialize(FVector extent, ConnectType type) override;
	virtual bool Update(float deltaTime);
	virtual void SetNeighbor(int x, int y, int z, UUnitComponent* unit) override;
	virtual void Trigger(TSubclassOf<UBeaconFire>& beaconFire) override;
	virtual bool IsTriggered() const { return b_NeedUpdate; }

public:
	UPROPERTY(VisibleAnyWhere)
		FVector m_UnitExtent;

private:
	UPROPERTY(VisibleAnyWhere)
		bool b_NeedUpdate = false;
	
	UPROPERTY(VisibleAnyWhere)
		float m_TotalBurningTime;
};
