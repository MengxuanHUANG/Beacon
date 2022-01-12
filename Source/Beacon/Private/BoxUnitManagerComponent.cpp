// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxUnitManagerComponent.h"

#include "BeaconMaterial.h"
#include "Curves/CurveFloat.h"
#include "UnitUpdater.h"

#include "BeaconLog.h"

UBoxUnitManagerComponent::UBoxUnitManagerComponent()
	:m_TriggeredUnits(UBoxUnitManagerComponent::CompareUnit)
{
}

void UBoxUnitManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	m_UnitUpdater = MakeShared<UnitUpdater>(m_Material);
}

// Called every frame
void UBoxUnitManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_UnitUpdater.IsValid())
	{
		m_UnitUpdater->UpdateUnit(DeltaTime, m_TriggeredUnits, m_BeaconFire, m_UnitCount.X * m_UnitCount.Y * m_UnitCount.Z);
	}
}

void UBoxUnitManagerComponent::OnUnregister()
{
	for (auto unit : m_Units)
	{
		unit->UnregisterComponent();
	}
	m_TriggeredUnits.Empty();
	m_Units.Empty();

	Super::OnUnregister();
}

UBoxUnitManagerComponent::~UBoxUnitManagerComponent()
{
}

void UBoxUnitManagerComponent::UpdateUnits()
{
}

UUnitComponent* UBoxUnitManagerComponent::GetUnit(FVector index)
{
	if (index.X * m_UnitCount.Y * m_UnitCount.Z + index.Y * m_UnitCount.Z + index.Z >= m_Units.Num())
	{
		return nullptr;
	}
	else
	{
		return m_Units[index.X * m_UnitCount.Y * m_UnitCount.Z + index.Y * m_UnitCount.Z + index.Z];
	}
}

void UBoxUnitManagerComponent::TriggerUnit(FVector index)
{
	UUnitComponent* unit;
	if ((unit = GetUnit(index)) != nullptr)
	{
		unit->Trigger(m_BeaconFire);
		m_TriggeredUnits.Push(unit);
	}
}

void UBoxUnitManagerComponent::SetParameter3(uint32 x, uint32 y, uint32 z)
{
	m_UnitCount.X = x;
	m_UnitCount.Y = y;
	m_UnitCount.Z = z;
}

void UBoxUnitManagerComponent::SetBeaconFire(TSubclassOf<UBeaconFire>& beaconFire)
{
	m_BeaconFire = beaconFire;
}

bool UBoxUnitManagerComponent::CompareUnit(UUnitComponent* a, UUnitComponent* b)
{
	return *a > *b;
}