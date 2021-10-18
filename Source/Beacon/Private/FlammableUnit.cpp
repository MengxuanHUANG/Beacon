// Fill out your copyright notice in the Description page of Project Settings.


#include "FlammableUnit.h"
#include "BeaconCore.h"

#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"

#include "Components/StaticMeshComponent.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Actor.h"

void Neighbors::SetNeighbor(int x, int y, int z, UFlammableUnit* unit)
{
	int index = GetIndex(x + 1, y + 1, z + 1);
	if (index >= 0 && index < neighbors.Num())
	{
		if (neighbors[index] == nullptr)
		{
			neighbors[index] = unit;
			unit->SetNeighbor(-x, -y, -z, self);
		}
	}
}
Neighbors::~Neighbors()
{
}

int SixDirNeighbors::GetIndex(int x, int y, int z)
{
	return ((x & 2) >> 1)
		+ ((y ^ 1) & 1) * 2 + ((y & 2) >> 1)
		+ ((z ^ 1) & 1) * 4 + ((z & 2) >> 1);
}
int TwentySixDirNeighbors::GetIndex(int x, int y, int z)
{
	return x + y * 3 + z * 9;
}
// Sets default values for this component's properties
UFlammableUnit::UFlammableUnit()
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
	m_UnitExtent = extent;
	m_ConnectType = type;
	m_ParticleSystem->RegisterComponent();

	switch (type)
	{
	case ConnectType::SixDirection:
		m_Neighbors = Neighbors::CreateNeighbors<SixDirNeighbors>(this);
		break;
	case ConnectType::TwentySixDirection:
		m_Neighbors = Neighbors::CreateNeighbors<TwentySixDirNeighbors>(this);
		break;
	}
	

#ifdef BEACON_DEBUG
	m_DebugBox->RegisterComponent();
	m_DebugBox->SetBoxExtent(m_UnitExtent);
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

void UFlammableUnit::UpdateExtent(FVector extent)
{
	m_UnitExtent = extent;
	m_DebugBox->SetBoxExtent(m_UnitExtent);
}

void UFlammableUnit::Ignite(UParticleSystem* particle)
{
	if (!b_IsBurning && m_ParticleSystem != nullptr)
	{
		m_ParticleSystem->SetTemplate(particle);
	}
}

void UFlammableUnit::SetNeighbor(int x, int y, int z, UFlammableUnit* unit)
{
	m_Neighbors->SetNeighbor(x, y, z, unit);
}

void UFlammableUnit::DisplayDebugInfo()
{
#ifdef BEACON_DEBUG
	FString name;
	GetName(name);
	UE_LOG(LogTemp, Warning, TEXT("%s has neighbors"), *name);
	for (int i = 0; i < int(m_ConnectType); i++)
	{
		if (m_Neighbors->neighbors[i] == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("index %d: nullptr"), i);
		}
		else
		{
			UFlammableUnit* unit = m_Neighbors->neighbors[i];
			unit->GetName(name);
			UE_LOG(LogTemp, Warning, TEXT("index %d: %s"), i, *name);
		}
	}
#endif
}