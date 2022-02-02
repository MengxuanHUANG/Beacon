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
	virtual void OnUnregister() override;
public:
	virtual void Initialize(UUnitManagerComponent* manager, FVector extent, ConnectType type) override;
	virtual void Update(float deltaTime);
	virtual void SetNeighbor(int x, int y, int z, UUnitComponent* unit) override;
	virtual void SetNeighbor(FVector direction, UUnitComponent* unit) override;
	virtual void Trigger(TSubclassOf<UBeaconFire>& beaconFire) override;
	virtual void UnTrigger() override;

public:
	UPROPERTY(VisibleAnyWhere)
		FVector m_UnitExtent;

private:
	UPROPERTY(VisibleAnyWhere)
		float m_TotalBurningTime;
};
