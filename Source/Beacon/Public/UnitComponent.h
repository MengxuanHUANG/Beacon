// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Neighbor.h"
#include "BeaconCore.h"
#include "UnitComponent.generated.h"

class UBeaconFire;
class UBeaconMaterial;
class UBoxComponent;
class UUnitManagerComponent;

#define BEACON_Bit(i) (1<<i)

UENUM(Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EUnitFlag : int8
{
	None = 0 UMETA(Hidden),
	NeedUpdate = BEACON_Bit(0),
	Triggered  = BEACON_Bit(1),
};
ENUM_CLASS_FLAGS(EUnitFlag);

struct UnitConnection
{
	UnitConnection(UUnitComponent* self, UUnitComponent* other, FVector dir)
		:Self(self), Other(other), Direction(dir)
	{}
	~UnitConnection()
	{
		Self = Other = nullptr;
	}

	UUnitComponent* Self;
	UUnitComponent* Other;
	FVector Direction;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEACON_API UUnitComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUnitComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnUnregister() override;

public:	
	virtual void Initialize(UUnitManagerComponent* manager, FVector extent, ConnectType type) {}
	virtual void Update(float deltaTime) {}

	virtual void Trigger() {}
	virtual void Trigger(TSubclassOf<UBeaconFire>& beaconFire) {}
	virtual void UnTrigger() {}

	virtual void SetNeighbor(int x, int y, int z, UUnitComponent* unit) {}
	virtual void SetNeighbor(FVector direction, UUnitComponent* unit) {}
	virtual UNeighbor* GetNeighbors() const { return m_Neighbors; }
	virtual void GetTemporaryNeighbors(TArray<TSharedPtr<UnitConnection>>& tempConnections) const {}
	virtual bool IsTriggered() const { return CheckFlag(EUnitFlag::Triggered); }

public:
	inline void AddValue(float value) { Value += value; }

	float GetTemperature() const;
	inline uint32 GetIndex() const { return m_Index; }
	inline void SetIndex(uint32 index) { m_Index = index; }
	inline void SetMaterial(UBeaconMaterial* material) { m_Material = material; }
	inline UBeaconMaterial* GetMaterial() const { return m_Material; }
	inline UUnitManagerComponent* GetManager() { return m_Manager; }

	inline void SetMaxBurningTime(float maxBurningTime) { m_MaxBurningTime = maxBurningTime; }
	inline float GetMaxBurningTime() const { return m_MaxBurningTime; }

	inline void SetFlag(EUnitFlag flag, bool isSet = true) { m_Flag = EUnitFlag(isSet ? int8(m_Flag) | int8(flag) : int8(m_Flag) & ~int8(flag)); }
	inline bool CheckFlag(EUnitFlag flag) const { return (int8(m_Flag) & int8(flag)) != 0; }

public:
	UFUNCTION()
		void SetReceiveThermalRadiation(bool enable);
public:
	//override operators for value
	virtual bool operator < (const UUnitComponent& unit);
	virtual bool operator <= (const UUnitComponent& unit);
	virtual bool operator > (const UUnitComponent& unit);
	virtual bool operator >= (const UUnitComponent& unit);
	virtual bool operator == (const UUnitComponent& unit);
	virtual bool operator != (const UUnitComponent& unit);

	virtual bool operator < (float value);
	virtual bool operator <= (float value);
	virtual bool operator > (float value);
	virtual bool operator >= (float value);
	virtual bool operator == (float value);
	virtual bool operator != (float value);

	virtual void IGetName(FString& name) {}

	virtual void DisplayDebugInfo() {}

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		ConnectType m_ConnectType;
	
	UPROPERTY(VisibleAnywhere)
		UNeighbor* m_Neighbors;

	UPROPERTY(VisibleAnyWhere)
		UBoxComponent* DebugBox;

	UPROPERTY(VisibleAnywhere)
		float Value = 0.f;
#ifdef BEACON_DEBUG
	// index for debug only
	FVector Debug_Index;

#endif
protected:
	UPROPERTY(VisibleAnywhere)
		uint32 m_Index;

	UPROPERTY(VisibleAnywhere)
		UUnitManagerComponent* m_Manager;

	UPROPERTY(VisibleAnyWhere)
		float m_MaxBurningTime;
	
	UPROPERTY(VisibleAnywhere)
		UBeaconMaterial* m_Material;

	UPROPERTY(VisibleAnywhere, Meta = (Bitmask, BitmaskEnum = EUnitFlag))
		EUnitFlag m_Flag = EUnitFlag::None;

};
