// Fill out your copyright notice in the Description page of Project Settings.


#include "FlammableUnitComponent.h"
#include "SixDirNeighbor.h"

#include "BeaconLog.h"

#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"

#include "Components/StaticMeshComponent.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Actor.h"

UFlammableUnitComponent::UFlammableUnitComponent()
{
	m_DebugBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Debug Box"));
	m_DebugBox->SetupAttachment(this);
	m_DebugBox->SetGenerateOverlapEvents(false);
	m_DebugBox->bHiddenInGame = false;

	m_ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	m_ParticleSystem->SetupAttachment(this);
	m_ParticleSystem->SetUsingAbsoluteRotation(true);
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


	m_DebugBox->RegisterComponent();
	m_DebugBox->SetBoxExtent(extent);
}

void UFlammableUnitComponent::Trigger(UParticleSystem* particle)
{
	if (!b_IsBurning && m_ParticleSystem != nullptr)
	{
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