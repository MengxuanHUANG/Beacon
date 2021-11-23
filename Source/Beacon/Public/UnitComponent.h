// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Neighbor.h"
#include "UnitComponent.generated.h"

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

	virtual void Initialize(FVector extent, ConnectType type) {}
	virtual void Trigger(UParticleSystem* particle) {}
	virtual void SetNeighbor(int x, int y, int z, UUnitComponent* unit) {}
	virtual UNeighbor* GetNeighbors() const { return m_Neighbors; }

	virtual bool IsTriggered() const { return false; }

	inline uint32 GetIndex() const { return m_Index; }
	inline void SetIndex(uint32 index) { m_Index = index; }
	inline float GetValue() const { return m_Value; }
	//override operators for value
	UUnitComponent& operator = (float value);
	UUnitComponent& operator + (float value);
	UUnitComponent& operator += (float value);
	UUnitComponent& operator - (float value);
	UUnitComponent& operator -= (float value);
	UUnitComponent& operator * (float value);
	UUnitComponent& operator *= (float value);
	UUnitComponent& operator / (float value);
	UUnitComponent& operator /= (float value);


	bool operator < (const UUnitComponent& unit);
	bool operator <= (const UUnitComponent& unit);
	bool operator > (const UUnitComponent& unit);
	bool operator >= (const UUnitComponent& unit);
	bool operator == (const UUnitComponent& unit);
	bool operator != (const UUnitComponent& unit);

	bool operator < (float value);
	bool operator <= (float value);
	bool operator > (float value);
	bool operator >= (float value);
	bool operator == (float value);
	bool operator != (float value);

	virtual void IGetName(FString& name) {}

	virtual void DisplayDebugInfo() {}
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		ConnectType m_ConnectType;
	
	UPROPERTY(VisibleAnywhere)
		UNeighbor* m_Neighbors;

protected:
	UPROPERTY(VisibleAnywhere)
		uint32 m_Index;

	UPROPERTY(VisibleAnywhere)
		float m_Value;
};
