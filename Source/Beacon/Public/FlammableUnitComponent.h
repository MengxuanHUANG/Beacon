// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitComponent.h"
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
public:
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly)
		UParticleSystemComponent* m_ParticleSystem;

	UPROPERTY(VisibleAnyWhere)
		UBoxComponent* m_DebugBox;

	UPROPERTY(VisibleAnyWhere)
		FVector m_UnitExtent;

public:
	virtual void Initialize(FVector extent, ConnectType type);
	virtual void Trigger(UParticleSystem* particle);
	virtual void SetNeighbor(int x, int y, int z, UUnitComponent* unit);
	virtual UNeighbor* GetNeighbors() const { return m_Neighbors; }

	virtual void DisplayDebugInfo();

public:
	virtual inline bool IsTriggered() const { return b_IsBurning; }

private:
	bool b_IsBurning;
};
