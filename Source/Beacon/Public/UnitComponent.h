// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Neighbor.h"
#include "UnitComponent.generated.h"

class UBeaconFire;
class UBoxComponent;
class UBeaconMaterial;
class UUnitManagerComponent;

#define BEACON_Bit(i) (1<<i)

enum UnitFlag : int8
{
	NeedUpdate = BEACON_Bit(0),
	Triggered  = BEACON_Bit(1),
};

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

public:	
	// Called every frame

	virtual void Initialize(UUnitManagerComponent* manager, FVector extent, ConnectType type) {}
	virtual void Update(float deltaTime) {}
	virtual void Trigger(TSubclassOf<UBeaconFire>& beaconFire) {}
	virtual void UnTrigger() {}
	virtual void SetNeighbor(int x, int y, int z, UUnitComponent* unit) {}
	virtual void SetNeighbor(FVector direction, UUnitComponent* unit) {}
	virtual UNeighbor* GetNeighbors() const { return m_Neighbors; }
	virtual void GetTemporaryNeighbors(TArray<TSharedPtr<UnitConnection>>& tempConnections) const {}
	virtual bool IsTriggered() const { return CheckFlag(UnitFlag::Triggered); }

public:
	float GetTemperature() const;
	inline uint32 GetIndex() const { return m_Index; }
	inline void SetIndex(uint32 index) { m_Index = index; }
	UBeaconMaterial* GetMaterial() const;
	UUnitManagerComponent* GetManager() { return m_Manager; }

	inline void SetFlag(UnitFlag flag, bool needUpdate = true) { m_Flag = (needUpdate ? m_Flag | flag : m_Flag & ~flag); }
	inline bool CheckFlag(UnitFlag flag) const { return (m_Flag & flag) != 0; }

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

protected:
	UPROPERTY(VisibleAnywhere)
		uint32 m_Index;

	UPROPERTY(VisibleAnywhere)
		UUnitManagerComponent* m_Manager;
	
	UPROPERTY(VisibleAnywhere)
		int8 m_Flag = 0;
};
