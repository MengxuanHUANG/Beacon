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
	virtual void Update(float deltaTime);
	virtual void Initialize(UUnitManagerComponent* manager, FVector extent, ConnectType type);
	
	virtual void Trigger();
	virtual void Trigger(TSubclassOf<UBeaconFire>& beaconFire);
	virtual void UnTrigger();
	
	virtual void SetNeighbor(int x, int y, int z, UUnitComponent* unit);
	virtual void SetNeighbor(FVector direction, UUnitComponent* unit);
	virtual UNeighbor* GetNeighbors() const { return m_Neighbors; }
	virtual void GetTemporaryNeighbors(TArray<TSharedPtr<UnitConnection>>& tempConnections) const;

public:
	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

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

	TMap<FString, TSharedPtr<UnitConnection>> m_TempConnections;
};
