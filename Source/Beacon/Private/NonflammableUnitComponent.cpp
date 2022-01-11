// Fill out your copyright notice in the Description page of Project Settings.


#include "NonflammableUnitComponent.h"
#include "SixDirNeighbor.h"

#include "BeaconLog.h"

#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"

#include "Particles/ParticleSystemComponent.h"

#include "DrawDebugHelpers.h"
#include "Misc/App.h"

void UNonflammableUnitComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UNonflammableUnitComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//TODO: wrapped in BEACON_DEBUG
	if (b_NeedUpdate)
	{
		DrawDebugBox(
			GetWorld(),
			GetComponentLocation(),
			m_UnitExtent - 1,
			FColor::Blue,
			false, DeltaTime, 0, 1);
	}
}

void UNonflammableUnitComponent::Initialize(FVector extent, ConnectType type)
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

void UNonflammableUnitComponent::SetNeighbor(int x, int y, int z, UUnitComponent* unit)
{
	m_Neighbors->SetNeighbor(x, y, z, unit);
}

void UNonflammableUnitComponent::Trigger(TSubclassOf<UBeaconFire>& beaconFire)
{
	b_NeedUpdate = true;
}

bool UNonflammableUnitComponent::IsTriggered() const
{
	return false;
}
