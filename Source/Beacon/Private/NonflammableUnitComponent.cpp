// Fill out your copyright notice in the Description page of Project Settings.


#include "NonflammableUnitComponent.h"
#include "SixDirNeighbor.h"

#include "BeaconLog.h"
#include "BeaconMaterial.h"
#include "UnitManagerComponent.h"

#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"

#include "Particles/ParticleSystemComponent.h"

#include "DrawDebugHelpers.h"
#include "Misc/App.h"

//BEACON Macro
#ifdef BEACON_DEBUG
	//Whether to hide box for UnitComponent
#define BEACON_DEBUG_BOX_VISIBLE true
#define BEACON_HIDE_DEBUG_BOX_IN_GAME true
#endif

void UNonflammableUnitComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UNonflammableUnitComponent::Initialize(UUnitManagerComponent* manager, FVector extent, ConnectType type)
{
	//Manager
	m_Manager = manager;

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
	UBeaconMaterial* material = GetMaterial();
	if (material != nullptr)
	{
		Value = material->DefaultThermal;
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

void UNonflammableUnitComponent::Update(float deltaTime)
{
	UBeaconMaterial* material = GetMaterial();
	BEACON_ASSERT(material != nullptr);

	if (CheckFlag(UnitFlag::NeedUpdate))
	{
		if (CheckFlag(UnitFlag::Triggered))
		{
			//reduce remainder burning time
			if (material->Has_Max_BurningTime)
			{
				m_TotalBurningTime += deltaTime;
			}
			//increase thermal energy
			if (Value < material->MAX_Thermal)
			{
				Value += deltaTime * float(material->GenThermalPerSecond);
			}
			//check whether to end burning
			if (m_TotalBurningTime >= material->Max_BurningTime)
			{
				SetFlag(UnitFlag::Triggered, false);
			}
		}

		//reduce thermal energy
		float loss = deltaTime * GetMaterial()->LoseThermalPerSecond;
		Value -= loss;

		if (Value >= material->Flash_Point)
		{
			Trigger(GetManager()->GetBeaconFire());
		}
		else if (Value < GetMaterial()->DefaultThermal)
		{
			//Stop update unit if its value smaller than default value
			Value = GetMaterial()->DefaultThermal;
			UnTrigger();
		}
	}
}

void UNonflammableUnitComponent::SetNeighbor(int x, int y, int z, UUnitComponent* unit)
{
	m_Neighbors->SetNeighbor(x, y, z, unit);
}

void UNonflammableUnitComponent::SetNeighbor(FVector direction, UUnitComponent* unit)
{
	SetNeighbor(direction.X, direction.Y, direction.Z, unit);
}

void UNonflammableUnitComponent::Trigger(TSubclassOf<UBeaconFire>& beaconFire)
{
	SetFlag(UnitFlag::Triggered);
	SetFlag(UnitFlag::NeedUpdate);
}

void UNonflammableUnitComponent::UnTrigger()
{
	SetFlag(UnitFlag::Triggered, false);
}

#ifdef BEACON_DEBUG_BOX_VISIBLE
#undef BEACON_DEBUG_BOX_VISIBLE
#endif

#ifdef BEACON_HIDE_DEBUG_BOX_IN_GAME
#undef BEACON_HIDE_DEBUG_BOX_IN_GAME
#endif