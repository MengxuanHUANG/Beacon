// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

#include "Neighbor.h"

#include "UnitManagerComponent.generated.h"

class USceneComponent;
class UUnitComponent;

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

	virtual void CreateUnits(USceneComponent* self, const USceneComponent* parent) {}

	virtual void TriggerUnit(FVector index) {}
	virtual UUnitComponent* GetUnit(FVector index) { return nullptr; }
	virtual void UpdateUnits() {}

	virtual void SetParticle(UParticleSystem* particle) {}

	virtual void SetParameter(uint32 x) {}
	virtual void SetParameter2(uint32 x, uint32 y) {}
	virtual void SetParameter3(uint32 x, uint32 y, uint32 z) {}

public:
	inline void SetConnectType(ConnectType type) { m_ConnectType = type; }
	inline ConnectType GetConnectType() const { return m_ConnectType; }

protected:
	ConnectType m_ConnectType;
};
