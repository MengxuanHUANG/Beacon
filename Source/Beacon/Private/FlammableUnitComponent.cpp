// Fill out your copyright notice in the Description page of Project Settings.


#include "FlammableUnitComponent.h"
#include "SixDirNeighbor.h"

#include "BeaconLog.h"

#include "BeaconMaterial.h"
#include "BeaconFire.h"
#include "UnitManagerComponent.h"

#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"

#include "DrawDebugHelpers.h"
#include "Misc/App.h"

//BEACON Macro
#ifdef BEACON_DEBUG
	//Whether to hide box for UnitComponent
#define BEACON_DEBUG_BOX_VISIBLE true
#define BEACON_HIDE_DEBUG_BOX_IN_GAME false
#endif

UFlammableUnitComponent::UFlammableUnitComponent()
{
}

void UFlammableUnitComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UFlammableUnitComponent::OnUnregister()
{
	if (m_BeaconFire != nullptr)
	{
		m_BeaconFire->DestroyComponent();
		m_BeaconFire = nullptr;
	}
	Super::OnUnregister();
}

void UFlammableUnitComponent::Initialize(UUnitManagerComponent* manager, FVector extent, ConnectType type)
{ 
	//Manager
	m_Manager = manager;

	//Debug Box
	m_UnitExtent = extent;
	DebugBox = NewObject<UBoxComponent>(this);
	DebugBox->RegisterComponent();

	DebugBox->ShapeColor = FColor::Red;
	DebugBox->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	DebugBox->SetVisibility(BEACON_DEBUG_BOX_VISIBLE);
	DebugBox->bHiddenInGame = BEACON_HIDE_DEBUG_BOX_IN_GAME;
	DebugBox->SetBoxExtent(extent);
	DebugBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	DebugBox->OnComponentBeginOverlap.AddDynamic(this, &UFlammableUnitComponent::OnBeginOverlap);
	DebugBox->OnComponentEndOverlap.AddDynamic(this, &UFlammableUnitComponent::OnEndOverlap);

	//Set tag to BoxComponent
	if (!(DebugBox->ComponentHasTag(BEACON_FLAMMABLE_UNIT_TAG)))
	{
		DebugBox->ComponentTags.Add(BEACON_FLAMMABLE_UNIT_TAG);
	}

	//burning parameters
	m_TotalBurningTime = 0.f;
	m_BurningEventCount = 0;
	UBeaconMaterial* material = GetMaterial();
	if (material)
	{
		Value = material->DefaultThermal;
	}
	
	//Connection
	m_ConnectType = type;
	switch (type)
	{
	case ConnectType::SixDirection:
		m_Neighbors = NewObject<USixDirNeighbor>(this);

		m_Neighbors->RegisterComponent();
		m_Neighbors->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
		break;
	case ConnectType::None:
		break;
	}
}

void UFlammableUnitComponent::Update(float deltaTime)
{
	UBeaconMaterial* material = GetMaterial();
	BEACON_ASSERT(material);

	if (CheckFlag(EUnitFlag::NeedUpdate))
	{
		if (CheckFlag(EUnitFlag::Triggered))
		{
			//increase burning time
			m_TotalBurningTime += deltaTime;

			//call burning events
			if (m_BurningEventCount < material->BurningEvents.Num())
			{
				FPair& event = material->BurningEvents[m_BurningEventCount];
				if (m_TotalBurningTime > event.Time)
				{
					m_BeaconFire->CallBurningEvent(event.FunctionName);
					m_BurningEventCount++;
				}
			}

			//increase thermal energy
			if (Value < material->MAX_Thermal)
			{
				Value += deltaTime * material->GenThermalPerSecond;
			}

			//check whether to end burning
			if (Value < material->Flash_Point || (material->Has_Max_BurningTime && m_TotalBurningTime >= m_MaxBurningTime))
			{
				m_BeaconFire->EndBurning();
				SetFlag(EUnitFlag::Triggered, false);
			}
		}
		else if(Value >= material->Flash_Point && (!material->Has_Max_BurningTime || m_TotalBurningTime < m_MaxBurningTime))
		{
			Trigger();
		}

		if (Value >= material->DefaultThermal)
		{
			//reduce thermal energy
			float loss = deltaTime * material->LoseThermalPerSecond;
			Value -= loss;
		}
		else
		{
			//Stop update unit if its value smaller than default value
			Value = material->DefaultThermal;
			SetFlag(EUnitFlag::NeedUpdate, false);
		}
	}
}

void UFlammableUnitComponent::Trigger()
{
	BEACON_ASSERT(m_Manager);
	Trigger(m_Manager->GetBeaconFire());
}

void UFlammableUnitComponent::Trigger(TSubclassOf<UBeaconFire>& beaconFire)
{
	if (!CheckFlag(EUnitFlag::Triggered))
	{
		m_BeaconFire = NewObject<UBeaconFire>(this, beaconFire);
		m_BeaconFire->RegisterComponent();

		m_BeaconFire->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
		m_BeaconFire->StartBurning();

		SetFlag(EUnitFlag::Triggered | EUnitFlag::NeedUpdate);

		//For visualize debug
		//DrawDebugBox(GetWorld(), DebugBox->GetComponentLocation(), DebugBox->GetUnscaledBoxExtent(), FColor::Red, true, -1, 0, 3);

		//TODO: maybe move to another placce
		TArray<TSharedPtr<UnitConnection>> tempConnections;
		m_TempConnections.GenerateValueArray(tempConnections);
		for (TSharedPtr<UnitConnection>& connection : tempConnections)
		{
			UUnitComponent* unit = connection->Other;
			unit->GetManager()->AddToUpdateList(unit);
		}
	}
}

void UFlammableUnitComponent::UnTrigger()
{
	if (CheckFlag(EUnitFlag::Triggered))
	{
		if (m_BeaconFire)
		{
			m_BeaconFire->EndBurning();
		}
		SetFlag(EUnitFlag::Triggered, false);
	}
}

void UFlammableUnitComponent::SetNeighbor(int x, int y, int z, UUnitComponent* unit)
{
	m_Neighbors->SetNeighbor(x, y, z, unit);
}

void UFlammableUnitComponent::SetNeighbor(FVector direction, UUnitComponent* unit)
{
	SetNeighbor(direction.X, direction.Y, direction.Z, unit);
}

void UFlammableUnitComponent::GetTemporaryNeighbors(TArray<TSharedPtr<UnitConnection>>& tempConnections) const
{
	m_TempConnections.GenerateValueArray(tempConnections);
}

void UFlammableUnitComponent::OnBeginOverlap(UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName(BEACON_FLAMMABLE_UNIT_TAG)))
	{
		UUnitComponent* other = Cast<UUnitComponent>(OtherComp->GetAttachParent());

		if (other->GetManager() != GetManager() && !(other->IsTriggered()))
		{
			FString actorName;
			FString compName;
			OtherActor->GetName(actorName);
			other->GetName(compName);

			FString id = actorName.Append("_" + compName);

			if (!m_TempConnections.Contains(id))
			{
				FVector dir = other->GetComponentLocation() - this->GetComponentLocation();
				m_TempConnections.Add(id, MakeShared<UnitConnection>(this, other, dir));

				if (CheckFlag(EUnitFlag::Triggered))
				{
					other->SetFlag(EUnitFlag::NeedUpdate);
					other->GetManager()->AddToUpdateList(other);
				}
			}
		}
	}
}

void UFlammableUnitComponent::OnEndOverlap(class UPrimitiveComponent* HitComp,
	class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(FName(BEACON_FLAMMABLE_UNIT_TAG)))
	{
		UUnitComponent* other = Cast<UUnitComponent>(OtherComp->GetAttachParent());

		if (other->GetManager() != GetManager())
		{
			FString actorName;
			FString compName;
			OtherActor->GetName(actorName);
			other->GetName(compName);
			FString id = actorName.Append("_" + compName);

			if (m_TempConnections.Contains(id))
			{
				m_TempConnections.Remove(id);
			}
		}
	}
}

#ifdef BEACON_DEBUG_BOX_VISIBLE
#undef BEACON_DEBUG_BOX_VISIBLE
#endif

#ifdef BEACON_HIDE_DEBUG_BOX_IN_GAME
#undef BEACON_HIDE_DEBUG_BOX_IN_GAME
#endif
