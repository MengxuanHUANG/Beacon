// Fill out your copyright notice in the Description page of Project Settings.


#include "NonflammableUnitComponent.h"
#include "SixDirNeighbor.h"

#include "BeaconLog.h"
#include "BeaconMaterial.h"

#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"

#include "Particles/ParticleSystemComponent.h"

#include "DrawDebugHelpers.h"
#include "Misc/App.h"

//BEACON Macro
#ifdef BEACON_DEBUG
	//Whether to hide box for UnitComponent
#define BEACON_DEBUG_BOX_VISIBLE true
#define BEACON_HIDE_DEBUG_BOX_IN_GAME false
#endif

void UNonflammableUnitComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UNonflammableUnitComponent::Initialize(FVector extent, ConnectType type)
{
	//Debug Box
	m_UnitExtent = extent;
	DebugBox = NewObject<UBoxComponent>(this);
	DebugBox->RegisterComponent();

	DebugBox->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	DebugBox->SetVisibility(BEACON_DEBUG_BOX_VISIBLE);
	DebugBox->bHiddenInGame = BEACON_HIDE_DEBUG_BOX_IN_GAME;
	DebugBox->SetBoxExtent(extent);
	
	DrawDebugBox(
		GetWorld(),
		GetComponentLocation(),
		m_UnitExtent - 1,
		FColor::Blue,
		true);

	//burning parameters
	m_TotalBurningTime = 0.f;
	if (m_Material)
	{
		Value = m_Material->DefaultThermal;
	}
	
	//Connection
	m_ConnectType = type;
	switch (type)
	{
	case ConnectType::SixDirection:
		m_Neighbors = NewObject<USixDirNeighbor>(this);

		m_Neighbors->RegisterComponent();
		m_Neighbors->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
		break;
	case ConnectType::None:
		break;
	}
}

bool UNonflammableUnitComponent::Update(float deltaTime)
{
	if (b_NeedUpdate)
	{
		//reduce remainder burning time
		if (m_Material->Has_Max_BurningTime)
		{
			m_TotalBurningTime += deltaTime;
		}
		//increase thermal energy
		if (Value < m_Material->MAX_Thermal)
		{
			Value += deltaTime * float(m_Material->GenThermalPerSecond);
		}
		//check whether to end burning
		if (m_TotalBurningTime >= m_Material->Max_BurningTime)
		{
			Value = -100;
			b_NeedUpdate = false;
			return false;
		}

		DrawDebugBox(
			GetWorld(),
			GetComponentLocation(),
			m_UnitExtent - 1,
			FColor::Blue,
			false, deltaTime, 0, 1);

		return true;
	}
	return false;
}

void UNonflammableUnitComponent::SetNeighbor(int x, int y, int z, UUnitComponent* unit)
{
	m_Neighbors->SetNeighbor(x, y, z, unit);
}

void UNonflammableUnitComponent::Trigger(TSubclassOf<UBeaconFire>& beaconFire)
{
	b_NeedUpdate = true;
}

void UNonflammableUnitComponent::UnTrigger()
{
	b_NeedUpdate = false;
}

#ifdef BEACON_DEBUG_BOX_VISIBLE
#undef BEACON_DEBUG_BOX_VISIBLE
#endif

#ifdef BEACON_HIDE_DEBUG_BOX_IN_GAME
#undef BEACON_HIDE_DEBUG_BOX_IN_GAME
#endif