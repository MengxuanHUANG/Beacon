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

	virtual void OnUnregister() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Initialize(FVector extent, ConnectType type);
	virtual void Trigger(UParticleSystem* particle);
	virtual void SetNeighbor(int x, int y, int z, UUnitComponent* unit);
	virtual UNeighbor* GetNeighbors() const { return m_Neighbors; }

	virtual void DisplayDebugInfo();

public:
	virtual bool IsTriggered() const override { return b_IsBurning; }

public:
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly)
		UParticleSystemComponent* m_ParticleSystem;

	UPROPERTY(VisibleAnyWhere)
		FVector m_UnitExtent;

private:
	bool b_IsBurning;
};
