// Fill out your copyright notice in the Description page of Project Settings.


#include "FlammableComponent.h"

#include "Beacon.h"
#include "BeaconLog.h"

#include "FlammableUnit.h"

#include "Engine/Engine.h"

#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"

#include "Components/StaticMeshComponent.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

#include "GameplayTagContainer.h"

#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Actor.h"

#include "BoxUnitsManager.h"
#include "SphereUnitsManager.h"

#include "Modules/ModuleManager.h"

// Sets default values for this component's properties
UFlammableComponent::UFlammableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetUsingAbsoluteScale(true);

	AActor* owner = GetOwner();
	if (owner != nullptr)
	{
		owner->Tags.Add(FName(BEACON_FLAMMABLE_TAG));
	}
}

// Called when the game starts
void UFlammableComponent::BeginPlay()
{
	Super::BeginPlay();
}

//Called when component destroyed
void UFlammableComponent::DestroyComponent(bool bPromoteChildren)
{
	AActor* owner = GetOwner();
	if (owner != nullptr)
	{
		owner->Tags.Remove(FName(BEACON_FLAMMABLE_TAG));
	}

	if (m_UnitsManager != nullptr)
	{
		m_UnitsManager->Destroy(bPromoteChildren);
		delete m_UnitsManager;
		m_UnitsManager = nullptr;
	}
	Super::DestroyComponent(bPromoteChildren);
}

// Called every frame
void UFlammableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_UnitsManager != nullptr)
	{
		m_UnitsManager->UpdateUnits();
	}
}

void UFlammableComponent::ClearUnits()
{
	if (m_UnitsManager != nullptr)
	{
		m_UnitsManager->Destroy(false);
		delete m_UnitsManager;
		m_UnitsManager = nullptr;
	}
}

void UFlammableComponent::CreateUnits()
{
	USceneComponent* parent = this->GetAttachParent();

	if (parent != nullptr && m_UnitsManager == nullptr)
	{
		UClass* uClass = parent->GetClass();
		FString name = uClass->GetFullGroupName(false);

		//Temp: Bind Dynamic Callback Function
		if (name.Compare("BoxComponent") == 0)
		{
			UBoxComponent* box = Cast<UBoxComponent>(parent);

			box->OnComponentBeginOverlap.RemoveDynamic(this, &UFlammableComponent::OnBeginOverlap);
			box->OnComponentBeginOverlap.AddDynamic(this, &UFlammableComponent::OnBeginOverlap);

			m_UnitsManager = UnitsManager::CreateUnitsManager<BoxUnitsManager<UFlammableUnit, UFlammableUnit> >(m_ConnectType, m_UnitCounts.X, m_UnitCounts.Y, m_UnitCounts.Z);
			m_UnitsManager->CreateUnits(this, parent);
		}
		else if(name.Compare("SphereComponent") == 0)
		{
			USphereComponent* sphere = Cast<USphereComponent>(parent);

			sphere->OnComponentBeginOverlap.RemoveDynamic(this, &UFlammableComponent::OnBeginOverlap);
			sphere->OnComponentBeginOverlap.AddDynamic(this, &UFlammableComponent::OnBeginOverlap);
			
			m_UnitsManager = UnitsManager::CreateUnitsManager<SphereUnitsManager<UFlammableUnit, UFlammableUnit> >(m_ConnectType, m_Count);
			m_UnitsManager->CreateUnits(this, parent);
		}
		else if (name.Compare("CapsuleComponent") == 0)
		{

		}
	}
	if (m_InitializeWithFlame)
	{
		//TODO: burn
	}
}

void UFlammableComponent::Ignited(UParticleSystem* particle)
{
	//TODO: burn
}

void UFlammableComponent::OnBeginOverlap(UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!b_IsBurning && OtherActor->ActorHasTag(FName("Flammable")))
	{
#ifdef BEACON_DEBUG
		UE_LOG(LogTemp, Warning, TEXT("Ignite"));
#endif

		UFlammableComponent* otherflammable = Cast<UFlammableComponent>(OtherActor->GetComponentByClass(UFlammableComponent::StaticClass()));
		if (otherflammable->IsBurning())
		{
			this->Ignited(otherflammable->GetFireParticle());
		}
	}
}