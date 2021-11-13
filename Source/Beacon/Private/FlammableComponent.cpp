// Fill out your copyright notice in the Description page of Project Settings.


#include "FlammableComponent.h"

//UE Header files
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

#include "GameFramework/Actor.h"

//Beacon Header files
#include "BeaconLog.h"
#include "FlammableUnitComponent.h"
#include "BoxUnitManagerComponent.h"

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

UFlammableComponent::~UFlammableComponent()
{
}

// Called when the game starts
void UFlammableComponent::BeginPlay()
{
	Super::BeginPlay();

	//trigger one unit
	if (InitializeWithFlame)
	{
		//TODO: StartBurning
	}
}

//Called when component destroyed
void UFlammableComponent::DestroyComponent(bool bPromoteChildren)
{
	AActor* owner = GetOwner();
	if (owner != nullptr)
	{
		owner->Tags.Remove(FName(BEACON_FLAMMABLE_TAG));
	}
	Super::DestroyComponent(bPromoteChildren);
}

// Called every frame
void UFlammableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UFlammableComponent::ClearUnits()
{
	// Unregister component
	//m_UnitManager->DestroyComponent();
	m_UnitManager = nullptr;
}

void UFlammableComponent::CreateUnits()
{
	USceneComponent* parent = this->GetAttachParent();

	if (parent != nullptr && m_UnitManager == nullptr)
	{
		UClass* uClass = parent->GetClass();
		FString name = uClass->GetFullGroupName(false);

		//Temp: Bind Dynamic Callback Function
		if (name.Compare("BoxComponent") == 0)
		{
			UBoxComponent* box = Cast<UBoxComponent>(parent);

			box->OnComponentBeginOverlap.RemoveDynamic(this, &UFlammableComponent::OnBeginOverlap);
			box->OnComponentBeginOverlap.AddDynamic(this, &UFlammableComponent::OnBeginOverlap);

			m_UnitManager = NewObject<UBoxUnitManagerComponent>(this);
			m_UnitManager->RegisterComponent();
			m_UnitManager->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

			m_UnitManager->SetConnectType(m_ConnectType);
			m_UnitManager->SetParameter3(m_UnitCount.X, m_UnitCount.Y, m_UnitCount.Z);
			m_UnitManager->SetParticle(T_FireParticle);
			UBoxUnitManagerComponent::CreateUnit<UFlammableUnitComponent, UFlammableUnitComponent>(
				Cast<UBoxUnitManagerComponent>(m_UnitManager), 
				this, 
				parent
			);
		}
		else if(name.Compare("SphereComponent") == 0)
		{
			USphereComponent* sphere = Cast<USphereComponent>(parent);

			sphere->OnComponentBeginOverlap.RemoveDynamic(this, &UFlammableComponent::OnBeginOverlap);
			sphere->OnComponentBeginOverlap.AddDynamic(this, &UFlammableComponent::OnBeginOverlap);

		}
		else if (name.Compare("CapsuleComponent") == 0)
		{
			UCapsuleComponent* capsule = Cast<UCapsuleComponent>(parent);

			capsule->OnComponentBeginOverlap.RemoveDynamic(this, &UFlammableComponent::OnBeginOverlap);
			capsule->OnComponentBeginOverlap.AddDynamic(this, &UFlammableComponent::OnBeginOverlap);

		}
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