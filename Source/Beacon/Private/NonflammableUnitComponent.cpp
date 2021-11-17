// Fill out your copyright notice in the Description page of Project Settings.


#include "NonflammableUnitComponent.h"
#include "SixDirNeighbor.h"

#include "BeaconLog.h"

#include "Components/SceneComponent.h"

#include "Particles/ParticleSystemComponent.h"

#include "DrawDebugHelpers.h"
#include "Misc/App.h"

void UNonflammableUnitComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//TODO: wrapped in BEACON_DEBUG
	DrawDebugBox(
		GetWorld(),
		GetComponentLocation(),
		m_UnitExtent - 1,
		FColor::Blue,
		false, DeltaTime, 0, 1);
}

void UNonflammableUnitComponent::Initialize(FVector extent, ConnectType type)
{
	m_UnitExtent = extent;
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

	DrawDebugBox(
		GetWorld(),
		GetComponentLocation(),
		m_UnitExtent - 1,
		FColor::Blue ,
		true, -1, 0, 1);
}

void UNonflammableUnitComponent::SetNeighbor(int x, int y, int z, UUnitComponent* unit)
{
	m_Neighbors->SetNeighbor(x, y, z, unit);
}

bool UNonflammableUnitComponent::IsTriggered() const
{
	return false;
}
