// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

#include "Neighbor.h"

#include "UnitManagerComponent.generated.h"

class UUnitComponent;
class UBeaconMaterial;
class UnitUpdater;
class UBeaconFire;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEACON_API UUnitManagerComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUnitManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual ~UUnitManagerComponent() {}

	
	virtual void TriggerUnit_Implementation(FVector index, float initValue) {}
	virtual void TriggerAllUnits_Implementation(float initValue) {}
	
	virtual void UnTriggerUnit_Implementation(FVector index, float value) {}
	virtual void UnTriggerAllUnits_Implementation(float value) {}

	virtual UUnitComponent* GetUnit(FVector index) { return nullptr; }
	virtual void UpdateUnits() {}

	virtual void SetBeaconFire(TSubclassOf<UBeaconFire>& beaconFire) {}

	virtual void SetParameter(uint32 x) {}
	virtual void SetParameter2(uint32 x, uint32 y) {}
	virtual void SetParameter3(uint32 x, uint32 y, uint32 z) {}


	virtual FVector LocalLocation2Index(FVector location) const { return FVector(0); }
public:
	inline void TriggerUnit(FVector index, float initValue = 0)
	{
		TriggerUnit_Implementation(index, initValue);
	}

	inline void TriggerAllUnits(float initValue = 0)
	{
		TriggerAllUnits_Implementation(initValue);
	}

	inline void UnTriggerUnit(FVector index, float value = 0)
	{
		UnTriggerUnit_Implementation(index, value);
	}

	inline void UnTriggerAllUnits(float value = 0)
	{
		UnTriggerAllUnits_Implementation(value);
	}

public:
	inline void SetConnectType(ConnectType type) { m_ConnectType = type; }
	inline ConnectType GetConnectType() const { return m_ConnectType; }

	inline void SetMaterial(UBeaconMaterial* material) { m_Material = material; }
	inline const UBeaconMaterial* GetMaterial() { return m_Material; }

protected:
	ConnectType m_ConnectType;
	TSharedPtr<UnitUpdater> m_UnitUpdater;

public:
	UPROPERTY(VisibleAnywhere)
		UBeaconMaterial* m_Material;
};
