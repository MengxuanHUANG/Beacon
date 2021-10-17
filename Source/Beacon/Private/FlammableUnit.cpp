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

void UFlammableUnit::Initialize(FVector extent)
{
	m_UnitExtent = extent;
	m_ParticleSystem->RegisterComponent();

#ifdef BEACON_DEBUG
	m_DebugBox->RegisterComponent();
	m_DebugBox->SetBoxExtent(m_UnitExtent);
#endif
}

// Called every frame
void UFlammableUnit::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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