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
#include "NonFlammableUnitComponent.h"
#include "BoxUnitManagerComponent.h"
#include "SphereUnitManagerComponent.h"
#include "CapsuleUnitManagerComponent.h"
#include "BeaconMaterial.h"

// Sets default values for this component's properties
UFlammableComponent::UFlammableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetUsingAbsoluteScale(true);

	//Set tag to actor
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
	if (InitializeWithFlame && m_UnitManager != nullptr)
	{
		m_UnitManager->SetBeaconFire(T_BeaconFire);
		//TODO: StartBurning
		/*for (float i = -10; i < 10; i++)
		{
			for (float j = -10; j < 10; j++)
			{
				for (float k = -10; k < 10; k++)
				{
					m_UnitManager->TriggerUnit(FVector(i, j, k));
				}
			}
		}*/
		m_UnitManager->TriggerUnit(FVector(0));
	}
}

//Called when component destroyed
void UFlammableComponent::DestroyComponent(bool bPromoteChildren)
{
	//Remove tag from actor
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
	if (m_UnitManager != nullptr)
	{
		m_UnitManager->UnregisterComponent();
		m_UnitManager = nullptr;
	}
}

void UFlammableComponent::CreateUnits()
{
	USceneComponent* parent = this->GetAttachParent();
	
	//TODO: move to a function
	if (parent != nullptr && m_UnitManager == nullptr)
	{
		UClass* uClass = parent->GetClass();
		FString name = uClass->GetFullGroupName(false);

		//Temp: Bind Dynamic Callback Function
		if (name.Compare("BoxComponent") == 0)
		{
			UBoxComponent* box = Cast<UBoxComponent>(parent);

			//Bind callback function to collider
			box->OnComponentBeginOverlap.RemoveDynamic(this, &UFlammableComponent::OnBeginOverlap);
			box->OnComponentBeginOverlap.AddDynamic(this, &UFlammableComponent::OnBeginOverlap);

			//create UnitsManager
			m_UnitManager = NewObject<UBoxUnitManagerComponent>(this);
			m_UnitManager->RegisterComponent();
			m_UnitManager->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

			//set necessary parameters
			m_UnitManager->SetMaterial(T_Material);
			m_UnitManager->SetConnectType(m_ConnectType);
			m_UnitManager->SetParameter3(m_UnitCount.X, m_UnitCount.Y, m_UnitCount.Z);
			
			//create units
			UBoxUnitManagerComponent::CreateUnit<UNonflammableUnitComponent, UFlammableUnitComponent>(
				Cast<UBoxUnitManagerComponent>(m_UnitManager), 
				this, 
				parent
			);
		}
		else if(name.Compare("SphereComponent") == 0)
		{
			USphereComponent* sphere = Cast<USphereComponent>(parent);

			//Bind callback function to collider
			sphere->OnComponentBeginOverlap.RemoveDynamic(this, &UFlammableComponent::OnBeginOverlap);
			sphere->OnComponentBeginOverlap.AddDynamic(this, &UFlammableComponent::OnBeginOverlap);
			
			//create UnitsManager
			m_UnitManager = NewObject<USphereUnitManagerComponent>(this);
			m_UnitManager->RegisterComponent();
			m_UnitManager->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
			
			//set necessary parameters
			m_UnitManager->SetMaterial(T_Material);
			m_UnitManager->SetConnectType(m_ConnectType);
			m_UnitManager->SetParameter(m_Count);
			
			//create units
			USphereUnitManagerComponent::CreateUnit<UNonflammableUnitComponent, UFlammableUnitComponent>(
				Cast<USphereUnitManagerComponent>(m_UnitManager),
				this,
				parent
				);
		}
		else if (name.Compare("CapsuleComponent") == 0)
		{
			UCapsuleComponent* capsule = Cast<UCapsuleComponent>(parent);

			//Bind callback function to collider
			capsule->OnComponentBeginOverlap.RemoveDynamic(this, &UFlammableComponent::OnBeginOverlap);
			capsule->OnComponentBeginOverlap.AddDynamic(this, &UFlammableComponent::OnBeginOverlap);
			
			//create UnitsManager
			m_UnitManager = NewObject<UCapsuleUnitManagerComponent>(this);
			m_UnitManager->RegisterComponent();
			m_UnitManager->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
			
			//set necessary parameters
			m_UnitManager->SetMaterial(T_Material);
			m_UnitManager->SetConnectType(m_ConnectType);
			m_UnitManager->SetParameter2(m_UnitCount.X, m_UnitCount.Y);
			
			//create units
			UCapsuleUnitManagerComponent::CreateUnit<UNonflammableUnitComponent, UFlammableUnitComponent>(
				Cast<UCapsuleUnitManagerComponent>(m_UnitManager),
				this,
				parent
				);
		}
	}

	//TODO: move to a function
	
	//Generate template Effect
	if (T_Material)
	{
		if (m_UnitManager)
		{
			m_UnitManager->SetMaterial(T_Material);
		}

		ObjectTemplate Template = T_Material->GetObjectTemplate();
		switch (Template)
		{
		case ObjectTemplate::None:
			break;
		case ObjectTemplate::AlwaysBurn:
			break;
		case ObjectTemplate::Break:
			break;
		case ObjectTemplate::Melt:
			break;
		default:
			BEACON_LOG(Warning, "Undefine Template Type!");
			BEACON_ASSERT(false);
			break;
		}
	}
}

void UFlammableComponent::Ignited(UBeaconFire* beaconFireWrapper)
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
			//this->Ignited(otherflammable->GetBeaconFire());
		}
	}
}