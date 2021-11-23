// Fill out your copyright notice in the Description page of Project Settings.


#include "FlammableUnitComponent.h"
#include "SixDirNeighbor.h"

#include "BeaconLog.h"

#include "Components/SceneComponent.h"

#include "Particles/ParticleSystemComponent.h"

#include "DrawDebugHelpers.h"
#include "Misc/App.h"

UFlammableUnitComponent::UFlammableUnitComponent()
{
	m_ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	m_ParticleSystem->SetupAttachment(this);
	m_ParticleSystem->SetUsingAbsoluteRotation(true);
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
	if (m_ParticleSystem != nullptr)
	{
		m_ParticleSystem->UnregisterComponent();
	}
	if (m_Neighbors != nullptr)
	{
		m_Neighbors->UnregisterComponent();
	}

	Super::OnUnregister();
}

void UFlammableUnitComponent::Initialize(FVector extent, ConnectType type)
{
	m_ParticleSystem->RegisterComponent();

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
		FColor::Red,
		true, -1, 0, 1);
}

void UFlammableUnitComponent::Trigger(UParticleSystem* particle)
{
	if (!b_IsBurning && m_ParticleSystem != nullptr)
	{
		FString name;
		GetName(name);
		m_Value = 1000.f;
		m_ParticleSystem->SetTemplate(particle);
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