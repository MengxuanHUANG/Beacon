// Fill out your copyright notice in the Description page of Project Settings.


#include "FlammableUnit.h"
#include "BeaconLog.h"

#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"

#include "Components/StaticMeshComponent.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UFlammableUnit::UFlammableUnit()
	:m_ParticleSystem(nullptr), m_Neighbors(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

#ifdef BEACON_DEBUG
	m_DebugBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Debug Box"));
	m_DebugBox->SetupAttachment(this);
	m_DebugBox->SetGenerateOverlapEvents(false);
	m_DebugBox->bHiddenInGame = false; 
#endif

	m_ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	m_ParticleSystem->SetupAttachment(this);
	m_ParticleSystem->SetUsingAbsoluteRotation(true);
}

// Called when the game starts
void UFlammableUnit::BeginPlay()
{
	Super::BeginPlay();
}

void UFlammableUnit::Initialize(FVector extent,ConnectType type)
{
	m_ParticleSystem->RegisterComponent();

	m_UnitExtent = extent;
	m_ConnectType = type;

	switch (type)
	{
	case ConnectType::SixDirection:
		m_Neighbors = Neighbors<UFlammableUnit>::CreateNeighbors<SixDirNeighbors<Unit> >(this);
		break;
	case ConnectType::TwentySixDirection:
		m_Neighbors = Neighbors<UFlammableUnit>::CreateNeighbors<TwentySixDirNeighbors<Unit> >(this);
		break;
	case ConnectType::None:
		break;
	}
	

#ifdef BEACON_DEBUG
	m_DebugBox->RegisterComponent();
	m_DebugBox->SetBoxExtent(extent);
#endif
}

// Called every frame
void UFlammableUnit::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//TODO: update neighbors
}

//Called when component destroyed
void UFlammableUnit::DestroyComponent(bool bPromoteChildren)
{
	if (m_DebugBox != nullptr)
	{
		m_DebugBox->DestroyComponent(bPromoteChildren);
	}

	if (m_ParticleSystem != nullptr)
	{
		m_ParticleSystem->DestroyComponent(bPromoteChildren);
	}

	if (m_Neighbors != nullptr)
	{
		delete m_Neighbors;
	}

	Super::DestroyComponent(bPromoteChildren);
}

void UFlammableUnit::Trigger(UParticleSystem* particle)
{
	if (!b_IsBurning && m_ParticleSystem != nullptr)
	{
		m_ParticleSystem->SetTemplate(particle);
		b_IsBurning = true;
	}
}

void UFlammableUnit::SetNeighbor(int x, int y, int z, Unit* unit)
{
	m_Neighbors->SetNeighbor(x, y, z, unit);
}

const TArray<Unit*>& UFlammableUnit::GetNeighbors() const
{
	return m_Neighbors->neighbors;
}

void UFlammableUnit::IGetName(FString& name)
{
	GetName(name);
}

#ifdef BEACON_DEBUG
void UFlammableUnit::DisplayDebugInfo()
{
	FString name;
	GetName(name);
	BEACON_LOG(Warning, "neighbors of %s", *name);

	for (int i = 0; i < int(m_ConnectType); i++)
	{
		if (m_Neighbors->neighbors[i] == nullptr)
		{
			BEACON_LOG(Warning, "index %d: nullptr", i);
		}
		else
		{
			Unit* unit = m_Neighbors->neighbors[i];
			unit->IGetName(name);
			BEACON_LOG(Warning, "index %d: %s", i, *name);
		}
	}
}
#endif