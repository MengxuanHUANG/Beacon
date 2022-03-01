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
#include "FractureComponent.h"
#include "ThermalRadiationComponent.h"

// Sets default values for this component's properties
UFlammableComponent::UFlammableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	//avoid morphing
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

	if (T_Material)
	{
		ConfigObjectTemplate(T_Material->GetObjectTemplate());
	}

	if (m_UnitManager != nullptr)
	{
		m_UnitManager->SetBeaconFire(T_BeaconFire);
		m_UnitManager->SetMaterial(T_Material);
		m_UnitManager->SetUnitsMaterial();
		m_UnitManager->SetThermalProxyNeedUpdate(EnableThermalProxy);
		if (InitializeWithFlame)
		{
			m_UnitManager->TriggerAllUnits(T_Material->Flash_Point);
		}
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
	if (m_UnitManager != nullptr)
	{
		m_UnitManager->DestroyComponent();
		m_UnitManager = nullptr;
	}
	Super::DestroyComponent(bPromoteChildren);
}

// Called every frame
void UFlammableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UFlammableComponent::BurnAll()
{
	if (m_UnitManager)
	{
		m_UnitManager->SetBeaconFire(T_BeaconFire);
		m_UnitManager->TriggerAllUnits(100);
	}
}

//Begin implementing BuildableComponent functions
bool UFlammableComponent::Build_Implement()
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

			//create UnitsManager
			m_UnitManager = NewObject<UBoxUnitManagerComponent>(this);
			m_UnitManager->RegisterComponent();
			m_UnitManager->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

			//set necessary parameters
			m_UnitManager->SetConnectType(m_ConnectType);
			m_UnitManager->SetParameter3(m_UnitCount.X, m_UnitCount.Y, m_UnitCount.Z);

			//create units
			UBoxUnitManagerComponent::CreateUnit<UNonflammableUnitComponent, UFlammableUnitComponent>(
				Cast<UBoxUnitManagerComponent>(m_UnitManager),
				this,
				parent
				);
		}
		else if (name.Compare("SphereComponent") == 0)
		{
			USphereComponent* sphere = Cast<USphereComponent>(parent);

			//create UnitsManager
			m_UnitManager = NewObject<USphereUnitManagerComponent>(this);
			m_UnitManager->RegisterComponent();
			m_UnitManager->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

			//set necessary parameters
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

			//create UnitsManager
			m_UnitManager = NewObject<UCapsuleUnitManagerComponent>(this);
			m_UnitManager->RegisterComponent();
			m_UnitManager->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

			//set necessary parameters
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

	return true;
}

void UFlammableComponent::Clear_Implement()
{
	if (m_UnitManager != nullptr)
	{
		m_UnitManager->DestroyComponent();
		m_UnitManager = nullptr;
	}
}
//End implementing BuildableComponent functions

UFractureComponent* UFlammableComponent::GetFractureComponent() const
{
	USceneComponent* parent = GetAttachParent();
	USceneComponent* fracture = parent->GetAttachParent();
	return Cast<UFractureComponent>(fracture);
}

void UFlammableComponent::ConfigObjectTemplate(ObjectTemplate objTemplate)
{
	switch (objTemplate)
	{
	case ObjectTemplate::None:
		break;
	case ObjectTemplate::AlwaysBurn:
	{
		InitializeWithFlame = true;
		break;
	}
	case ObjectTemplate::Break:
	{
		UFractureComponent* fractureComp = GetFractureComponent();
		if (fractureComp)
		{
			BEACON_LOG(Error, "Must have FractureComponent to support break effect");
			fractureComp->OnFracturedEvent.AddDynamic(this, &UFlammableComponent::OnFractured);
			m_UnitManager->SetThermalProxyNeedUpdate(true);
			fractureComp->BindBeaconThermalData(m_UnitManager->GetBeaconThermalData());
		}
		break;
	}
	case ObjectTemplate::Melt:
		break;
	default:
		BEACON_LOG(Warning, "Undefine Template Type!");
		BEACON_ASSERT(false);
		break;
	}
}

void UFlammableComponent::OnFractured(bool needUpdate)
{
	BEACON_LOG(Warning, "Fractured");
	Clear_Implement();
}