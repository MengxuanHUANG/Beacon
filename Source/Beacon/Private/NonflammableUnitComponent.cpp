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
//Whether to hide box for UnitComponent
#define BEACON_DEBUG_BOX_VISIBLE true
#define BEACON_HIDE_DEBUG_BOX_IN_GAME true

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

	DebugBox->ShapeColor = FColor::Blue;
	DebugBox->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	DebugBox->SetVisibility(BEACON_DEBUG_BOX_VISIBLE);
	DebugBox->bHiddenInGame = BEACON_HIDE_DEBUG_BOX_IN_GAME;
	DebugBox->SetBoxExtent(extent);
	
	if (!(DebugBox->ComponentHasTag(BEACON_DEBUG_BOX)))
	{
		DebugBox->ComponentTags.Add(BEACON_DEBUG_BOX);
	}

	//Disable all overlap event with this object
	DebugBox->SetGenerateOverlapEvents(false);

	/*DrawDebugBox(
		GetWorld(),
		GetComponentLocation(),
		m_UnitExtent - 1,
		FColor::Blue,
		true);*/

	//burning parameters
	m_TotalBurningTime = 0.f;
	UBeaconMaterial* material = GetMaterial();
	if (material)
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
	BEACON_ASSERT(material);

	if (CheckFlag(EUnitFlag::NeedUpdate))
	{
		//reduce thermal energy
		float loss = deltaTime * material->LoseThermalPerSecond;
		Value -= loss;

		if (CheckFlag(EUnitFlag::Triggered))
		{
			//increase burning time
			m_TotalBurningTime += deltaTime;

			//increase thermal energy
			if (Value < material->MAX_Thermal)
			{
				Value += deltaTime * material->GenThermalPerSecond;
			}
			//check whether to end burning
			if (Value < material->Flash_Point || (material->Has_Max_BurningTime && m_TotalBurningTime >= m_MaxBurningTime))
			{
				SetFlag(EUnitFlag::Triggered, false);
			}
		}
		else if (m_TotalBurningTime < m_MaxBurningTime && Value >= material->Flash_Point)
		{
			SetFlag(EUnitFlag::Triggered);
		}

		if (Value <= material->DefaultThermal)
		{
			//Stop update unit if its value smaller than default value
			Value = material->DefaultThermal;
			SetFlag(EUnitFlag::NeedUpdate, false);
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

void UNonflammableUnitComponent::Trigger()
{
	Value = Value > m_Material->Flash_Point ? Value : m_Material->Flash_Point;
	SetFlag(EUnitFlag::Triggered | EUnitFlag::NeedUpdate);
}

void UNonflammableUnitComponent::Trigger(TSubclassOf<UBeaconFire>& beaconFire)
{
	Trigger();
}

void UNonflammableUnitComponent::UnTrigger()
{
	Value = GetMaterial()->Flash_Point - 0.01f;
	SetFlag(EUnitFlag::Triggered, false);
}

#ifdef BEACON_DEBUG_BOX_VISIBLE
#undef BEACON_DEBUG_BOX_VISIBLE
#endif

#ifdef BEACON_HIDE_DEBUG_BOX_IN_GAME
#undef BEACON_HIDE_DEBUG_BOX_IN_GAME
#endif