// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BeaconConnection.h"

class UBoxComponent;

/**
 * 
 */
class BEACON_API Unit
{
public:
	Unit() {}
	Unit(float value)
		:m_Value(value)
	{}
	virtual ~Unit() {};

	virtual void Initialize(FVector extent, ConnectType type) = 0;
	virtual void Trigger(UParticleSystem* particle) = 0;
	virtual void SetNeighbor(int x, int y, int z, Unit* unit) = 0;
	virtual const TArray<Unit*>& GetNeighbors() const = 0;

	virtual inline bool IsTriggered() const { return false; }

	inline float GetValue() const { return m_Value; }
	
	//override operators for value
	Unit& operator = (const Unit& otherUnit);
	Unit& operator = (float value);
	Unit& operator + (float value);
	Unit& operator += (float value);
	Unit& operator - (float value);
	Unit& operator -= (float value);
	Unit& operator * (float value);
	Unit& operator *= (float value);
	Unit& operator / (float value);
	Unit& operator /= (float value);

	virtual void IGetName(FString& name) {}

	virtual void DisplayDebugInfo() {}
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		ConnectType m_ConnectType;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float m_Value;
};
