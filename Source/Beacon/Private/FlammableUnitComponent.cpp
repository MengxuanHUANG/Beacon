// Fill out your copyright notice in the Description page of Project Settings.


#include "FlammableUnitComponent.h"
#include "SixDirNeighbor.h"

#include "BeaconLog.h"

#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"

#include "DrawDebugHelpers.h"
#include "Misc/App.h"

UFlammableUnitComponent::UFlammableUnitComponent()
{
}

void UFlammableUnitComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UFlammableUnitComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (b_IsBurning)
	{
		DrawDebugBox(
			GetWorld(),
			GetComponentLocation(),
			m_UnitExtent - 1,
			FColor::Red,
			false, DeltaTime, 0, 1);
	}
}

void UFlammableUnitComponent::OnUnregister()
{
	if (m_BeaconFire != nullptr)
	{
		m_BeaconFire->UnregisterComponent();
	}
	if (m_Neighbors != nullptr)
	{
		m_Neighbors->UnregisterComponent();
	}

	Super::OnUnregister();
}

void UFlammableUnitComponent::Initialize(FVector extent, ConnectType type)
{
	m_UnitExtent = extent;
	m_ConnectType = type; 
	DebugBox = NewObject<UBoxComponent>(this);
	DebugBox->RegisterComponent();

	DebugBox->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	DebugBox->SetVisibility(BEACON_HIDE_DEBUG_BOX);
	DebugBox->bHiddenInGame = BEACON_HIDE_DEBUG_BOX_IN_GAME;
	DebugBox->SetBoxExtent(extent);

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

void UFlammableUnitComponent::Trigger(TSubclassOf<UBeaconFire>& beaconFire)
{
	if (!b_IsBurning)
	{
		m_Value = 1000.f;
		m_BeaconFire = NewObject<UBeaconFire>(this, beaconFire);
		m_BeaconFire->RegisterComponent();

		m_BeaconFire->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
		m_BeaconFire->StartBurning();

		b_IsBurning = true;
	}
}

void UFlammableUnitComponent::SetNeighbor(int x, int y, int z, UUnitComponent* unit)
{
	m_Neighbors->SetNeighbor(x, y, z, unit);
}

void UFlammableUnitComponent::DisplayDebugInfo()
{

}