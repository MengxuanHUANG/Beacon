// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neighbor.h"
#include "BeaconThermalProxy.h"
#include "UnitManagerComponent.generated.h"

class UUnitComponent;
class UnitUpdater;
class UBeaconFire;
class UBeaconMaterial;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEACON_API UUnitManagerComponent : public USceneComponent, public BeaconThermalProxy
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUnitManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnUnregister() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual ~UUnitManagerComponent() {}
	
	virtual void AddToUpdateList(UUnitComponent* unit) {}

	virtual void TriggerUnit_Implementation(UUnitComponent* unit) {}
	virtual void TriggerUnit_Implementation(FVector index, float initValue) {}
	virtual void TriggerAllUnits_Implementation(float initValue) {}
	
	virtual void UnTriggerUnit_Implementation(FVector index, float value) {}
	virtual void UnTriggerAllUnits_Implementation(float value) {}

	virtual UUnitComponent* GetUnit(FVector index) { return nullptr; }

	virtual void SetReceiveThermalRadiation(bool enable) {}

	virtual void SetParameter(uint32 x) {}
	virtual void SetParameter2(uint32 x, uint32 y) {}
	virtual void SetParameter3(uint32 x, uint32 y, uint32 z) {}
	virtual void SetUnitsMaterial() {}

public:
	inline void TriggerUnit(UUnitComponent* unit)
	{
		TriggerUnit_Implementation(unit);
	}

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
	inline void SetBeaconFire(TSubclassOf<UBeaconFire>& beaconFire) { T_BeaconFire = beaconFire; }
	inline TSubclassOf<UBeaconFire>& GetBeaconFire() { return T_BeaconFire; }

	inline void SetConnectType(ConnectType type) { m_ConnectType = type; }
	inline ConnectType GetConnectType() const { return m_ConnectType; }

	inline void SetMaterial(const UBeaconMaterial* material) 
	{ 
		m_Material = DuplicateObject<UBeaconMaterial>(material, this);
	}
	inline UBeaconMaterial* GetMaterial() { return m_Material; }

protected:
	ConnectType m_ConnectType;
	TSharedPtr<UnitUpdater> m_UnitUpdater;

public:
	UBeaconMaterial* m_Material;
	
	UPROPERTY(VisibleAnywhere)
		TSubclassOf<UBeaconFire> T_BeaconFire;
};
