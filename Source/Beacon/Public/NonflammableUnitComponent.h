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
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Initialize(FVector extent, ConnectType type) override;
	virtual void SetNeighbor(int x, int y, int z, UUnitComponent* unit) override;

	virtual bool IsTriggered() const override;

public:
	UPROPERTY(VisibleAnyWhere)
		FVector m_UnitExtent;

private:
	bool b_NeedUpdate = false;
};
