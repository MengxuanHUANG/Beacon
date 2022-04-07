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
	if (m_ThermalRadiationComponent != nullptr)
	{
		m_ThermalRadiationComponent->SetRadiationEnable(bEnableSendThermalRadiation);

		if (EnableThermalProxy)
		{
			m_ThermalRadiationComponent->BindBeaconThermalData(m_UnitManager->GetBeaconThermalData());
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
	if (m_ThermalRadiationComponent != nullptr)
	{
		m_ThermalRadiationComponent->DestroyComponent();
		m_ThermalRadiationComponent = nullptr;
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

//Begin implementing ICheckInterface functions
bool UFlammableComponent::BeaconCheck_Implementation(FString& Info, FString& Message)
{
	BEACON_CLEAR_FSTRING(Info);
	BEACON_CLEAR_FSTRING(Message);

	BEACON_AUTO_FILL_INFO(Info);

	FString errors;
	if (!bIsBuilded)
	{
		errors.Append("Error: FlammableComponent has not been built! Please use BeaconEditor to build it!\n");
	}
	if (T_Material == nullptr)
	{
		errors.Append("Error: BeaconMaterial is Reuqired!\n");
	}
	if (T_BeaconFire.Get() == nullptr)
	{
		errors.Append("Error: Beacon Fire is Reuqired!\n");
	}

	if (errors.IsEmpty())
	{
		FString name = GetOwner()->GetName();
		Message.Append(name);
		Message.Append(": FlammableComponent pass runtime check.");
		return true;
	}
	else
	{
		Message.Append(errors);
		return false;
	}
}
//End implementing ICheckInterface functions

//Begin implementing BuildableComponent functions
bool UFlammableComponent::Build_Implement()
{
	USceneComponent* parent = this->GetAttachParent();

	//TODO: move to a function
	if (parent != nullptr && m_UnitManager == nullptr)
	{
		UClass* uClass = parent->GetClass();
		FString name = uClass->GetFullGroupName(false);

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
		else //Invalid parent component
		{
			BEACON_LOG_FULL(Error, "Invalid attached parent component!");
		}

		// Create Thermal Radiation
		if (bEnableSendThermalRadiation)
		{
			m_ThermalRadiationComponent = NewObject<UThermalRadiationComponent>(this);
			m_ThermalRadiationComponent->RegisterComponent();
			m_ThermalRadiationComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

			m_ThermalRadiationComponent->Initialize(T_RadiationMaterial, false);
		}

		// Let units be able to reveice radiation
		if (bEnableReceiveThermalRadiation)
		{
			m_UnitManager->SetReceiveThermalRadiation(true);
		}
		
		return true;
	}
	else
	{
		BEACON_LOG(Warning, "UFlammableComponent build failed!");
		return false;
	}
}

void UFlammableComponent::Clear_Implement()
{
	if (m_UnitManager != nullptr)
	{
		m_UnitManager->DestroyComponent();
		m_UnitManager = nullptr;
	}

	if (m_ThermalRadiationComponent != nullptr)
	{
		m_ThermalRadiationComponent->DestroyComponent();
		m_ThermalRadiationComponent = nullptr;
	}
}
//End implementing BuildableComponent functions

UFractureComponent* UFlammableComponent::GetFractureComponent() const
{
	UActorComponent* fracture = GetOwner()->GetComponentByClass(UFractureComponent::StaticClass());
	return (fracture ? Cast<UFractureComponent>(fracture) : nullptr);
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