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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Initialize(FVector extent, ConnectType type) {}
	virtual void Trigger(UParticleSystem* particle) {}
	virtual void SetNeighbor(int x, int y, int z, UUnitComponent* unit) {}
	virtual UNeighbor* GetNeighbors() const { return m_Neighbors; }

	virtual inline bool IsTriggered() const { return false; }

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

	virtual void IGetName(FString& name) {}

	virtual void DisplayDebugInfo() {}
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		ConnectType m_ConnectType;
	
	UPROPERTY(VisibleAnywhere)
		UNeighbor* m_Neighbors;

private:
	UPROPERTY(VisibleAnywhere)
		float m_Value;
};
