// Fill out your copyright notice in the Description page of Project Settings.


#include "BeaconFire_ParticleSystem.h"

#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"

#include "BeaconLog.h"
UBeaconFire_ParticleSystem::UBeaconFire_ParticleSystem()
{
	PrimaryComponentTick.bCanEverTick = true;

	m_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Fire Particles"));
	m_AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Fire Sound"));
}

void UBeaconFire_ParticleSystem::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}
void UBeaconFire_ParticleSystem::OnUnregister()
{
	if (m_Particle)
	{
		m_Particle->DestroyComponent();
		m_Particle = nullptr;
	}
	if (m_AudioComponent)
	{
		m_AudioComponent->DestroyComponent();
		m_AudioComponent = nullptr;
	}
	Super::OnUnregister();
}

void UBeaconFire_ParticleSystem::Initialize()
{
	m_Particle->RegisterComponent();
	m_Particle->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
}