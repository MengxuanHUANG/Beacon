// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxUnitManagerComponent.h"

#include "BeaconMaterial.h"
#include "Curves/CurveFloat.h"
#include "UnitUpdater.h"

#include "BeaconLog.h"

UBoxUnitManagerComponent::UBoxUnitManagerComponent()
	:m_UpdateList(UBoxUnitManagerComponent::CompareUnit)
{
}

void UBoxUnitManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	m_UnitUpdater = MakeShared<UnitUpdater>();
}

// Called every frame
void UBoxUnitManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_UnitUpdater.IsValid())
	{
		m_UnitUpdater->UpdateUnit(DeltaTime, m_UpdateList, T_BeaconFire, m_Units.Num());
	}
}

void UBoxUnitManagerComponent::OnUnregister()
{
	for (auto unit : m_Units)
	{
		if (unit)
		{
			unit->DestroyComponent();
			unit = nullptr;
		}
	}
	m_UpdateList.Empty();
	m_Units.Empty();

	Super::OnUnregister();
}

UBoxUnitManagerComponent::~UBoxUnitManagerComponent()
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

void UBoxUnitManagerComponent::AddToUpdateList(UUnitComponent* unit)
{
	if (unit && unit->GetManager() == this)
	{
		m_UpdateList.Push(unit);
	}
}

void UBoxUnitManagerComponent::TriggerUnit_Implementation(UUnitComponent* unit)
{
	unit->Trigger(T_BeaconFire);
	m_UpdateList.Push(unit);
}

void UBoxUnitManagerComponent::TriggerUnit_Implementation(FVector index, float initValue)
{
	UUnitComponent* unit;
	if ((unit = GetUnit(index)) != nullptr)
	{
		unit->Value = initValue;
		unit->Trigger(T_BeaconFire);
		m_UpdateList.Push(unit);
	}
}

void UBoxUnitManagerComponent::TriggerAllUnits_Implementation(float initValue)
{
	for (UUnitComponent* unit : m_Units)
	{
		unit->Value = initValue;
		unit->Trigger(T_BeaconFire);

		m_UpdateList.Push(unit);
	}
}

void UBoxUnitManagerComponent::UnTriggerUnit_Implementation(FVector index, float value)
{
	UUnitComponent* unit;
	if ((unit = GetUnit(index)) != nullptr)
	{
		unit->Value = value;
		unit->UnTrigger();

		//no need to mannully remove the unit from Priority Queue
	}
}

void UBoxUnitManagerComponent::UnTriggerAllUnits_Implementation(float value)
{
	for (UUnitComponent* unit : m_Units)
	{
		unit->Value = value;
		unit->UnTrigger();

		m_UpdateList.Empty();
	}
}

void UBoxUnitManagerComponent::SetParameter3(uint32 x, uint32 y, uint32 z)
{
	m_UnitCount.X = x;
	m_UnitCount.Y = y;
	m_UnitCount.Z = z;
}

void UBoxUnitManagerComponent::SetUnitsMaterial()
{
	for (UUnitComponent* unit : m_Units)
	{
		unit->SetMaterial(m_Material);
		if (m_Material->Has_Max_BurningTime)
		{
			unit->SetMaxBurningTime(FMath::RandRange(m_Material->Min_BurningTime, m_Material->Max_BurningTime));
		}
	}
}

bool UBoxUnitManagerComponent::CompareUnit(UUnitComponent* a, UUnitComponent* b)
{
	return *a > *b;
}