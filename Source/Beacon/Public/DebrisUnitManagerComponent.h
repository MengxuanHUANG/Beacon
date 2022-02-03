// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitManagerComponent.h"
#include "FlammableUnitComponent.h"
#include "DebrisUnitManagerComponent.generated.h"

class UFractureComponent;

/**
 * 
 */
UCLASS()
class BEACON_API UDebrisUnitManagerComponent : public UUnitManagerComponent
{
	GENERATED_BODY()
public:
	UDebrisUnitManagerComponent();
	~UDebrisUnitManagerComponent();
public:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnUnregister() override;

public:
	void Initialize(UFractureComponent*  fractureComponent, int32 rootIndex, bool isFractured = false);

private:
	UFUNCTION()
		void OnFractured();

public:
	UPROPERTY(VisibleAnywhere)
		UFractureComponent* m_FractureComponent;

	UPROPERTY(VisibleAnywhere)
		TMap<int32, UFlammableUnitComponent*> m_FlammableUnits;

private:
	bool bIsFractured;
};
