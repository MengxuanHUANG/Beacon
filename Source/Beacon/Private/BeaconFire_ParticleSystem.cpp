// Fill out your copyright notice in the Description page of Project Settings.


#include "BeaconFire_ParticleSystem.h"

#include "Particles/ParticleSystemComponent.h"
#include "BeaconLog.h"
UBeaconFire_ParticleSystem::UBeaconFire_ParticleSystem()
{
	PrimaryComponentTick.bCanEverTick = true;

	m_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Fire Particles"));
}

void UBeaconFire_ParticleSystem::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void UBeaconFire_ParticleSystem::Initialize()
{
	m_Particle->RegisterComponent();
	m_Particle->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
}