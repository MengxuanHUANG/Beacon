// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitManagerComponent.h"
#include "Components/SphereComponent.h"
#include "SphereUnitManagerComponent.generated.h"

/**
 * 
 */
UCLASS()
class BEACON_API USphereUnitManagerComponent : public UUnitManagerComponent
{
	GENERATED_BODY()

public:
	USphereUnitManagerComponent();
	~USphereUnitManagerComponent();

	virtual void UpdateUnits() override;
	virtual UUnitComponent* GetUnit(FVector index) override;
	virtual void TriggerUnit(FVector index) override;
	virtual void SetParameter(uint32 x) override;

	virtual void SetParticle(UParticleSystem* particle) override;

public:
	uint32 m_Count;
	

	UPROPERTY(VisibleAnywhere)
		TMap<FVector, UUnitComponent*> m_Units;

	TQueue<UUnitComponent*> m_TriggeredUnits;
	UParticleSystem* m_Particle;

public:
	template<typename InnerUnitType, typename OuterUnitType>
	static void CreateUnit(USphereUnitManagerComponent* sphereUnitManager, USceneComponent* self, const USceneComponent* parent)
	{

	}
};
