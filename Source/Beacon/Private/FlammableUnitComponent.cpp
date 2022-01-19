// Fill out your copyright notice in the Description page of Project Settings.


#include "FlammableUnitComponent.h"
#include "SixDirNeighbor.h"

#include "BeaconLog.h"

#include "BeaconMaterial.h"
#include "BeaconFire.h"

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
		m_BeaconFire->UnregisterComponent();
	}
	if (m_Neighbors != nullptr)
	{
		m_Neighbors->UnregisterComponent();
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

	DebugBox->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	DebugBox->SetVisibility(BEACON_DEBUG_BOX_VISIBLE);
	DebugBox->bHiddenInGame = BEACON_HIDE_DEBUG_BOX_IN_GAME;
	DebugBox->SetBoxExtent(extent);

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
	if (m_Material)
	{
		Value = m_Material->DefaultThermal;
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

bool UFlammableUnitComponent::Update(float deltaTime)
{
	if (b_IsBurning)
	{
		//reduce remainder burning time
		m_TotalBurningTime += deltaTime;
		//BEACON_LOG(Warning, "time is : %f", m_TotalBurningTime);
		
		//call burning events
		if (m_BurningEventCount < m_Material->BurningEvents.Num())
		{
			FPair& event = m_Material->BurningEvents[m_BurningEventCount];
			if (m_TotalBurningTime > event.Time)
			{
				m_BeaconFire->CallBurningEvent(event.FunctionName);
				m_BurningEventCount++;
			}
		}

		//increase thermal energy
		if (Value < m_Material->MAX_Thermal)
		{
			Value += deltaTime * float(m_Material->GenThermalPerSecond);
		}

		//check whether to end burning
		if (m_Material->Has_Max_BurningTime && m_TotalBurningTime >= m_Material->Max_BurningTime)
		{
			Value = -100;
			b_IsBurning = false;
			m_BeaconFire->EndBurning();
			return false;
		}
		/*DrawDebugBox(
			GetWorld(),
			GetComponentLocation(),
			m_UnitExtent - 1,
			FColor::Red,
			false, deltaTime, 0, 1);*/

		return true;
	}
	return false;
}

void UFlammableUnitComponent::Trigger(TSubclassOf<UBeaconFire>& beaconFire)
{
	if (!b_IsBurning)
	{
		m_BeaconFire = NewObject<UBeaconFire>(this, beaconFire);
		m_BeaconFire->RegisterComponent();

		m_BeaconFire->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
		m_BeaconFire->StartBurning();

		b_IsBurning = true;
	}
}

void UFlammableUnitComponent::UnTrigger()
{
	if (b_IsBurning)
	{
		if (m_BeaconFire)
		{
			m_BeaconFire->EndBurning();
		}

		b_IsBurning = false;
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

void UFlammableUnitComponent::DisplayDebugInfo()
{

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
		if (!(other->IsTriggered()))
		{
			FString actorName;
			FString compName;
			OtherActor->GetName(actorName);
			other->GetName(compName);

			FVector dir = other->GetComponentLocation() - this->GetComponentLocation();
			m_TempConnections.Add(actorName.Append("_" + compName), MakeShared<UnitConnection>(this, other, dir));
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
		FString actorName;
		FString compName;
		OtherActor->GetName(actorName);
		other->GetName(compName);

		m_TempConnections.Remove(actorName.Append("_" + compName));
	}
}

#ifdef BEACON_DEBUG_BOX_VISIBLE
#undef BEACON_DEBUG_BOX_VISIBLE
#endif

#ifdef BEACON_HIDE_DEBUG_BOX_IN_GAME
#undef BEACON_HIDE_DEBUG_BOX_IN_GAME
#endif
