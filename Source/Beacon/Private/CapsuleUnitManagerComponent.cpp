// Fill out your copyright notice in the Description page of Project Settings.


#include "CapsuleUnitManagerComponent.h"
#include "BeaconMaterial.h"
#include "Curves/CurveFloat.h"
#include "UnitComponent.h"
#include "UnitUpdater.h"
#include "BeaconLog.h"

UCapsuleUnitManagerComponent::UCapsuleUnitManagerComponent()
	:m_UpdateList(UCapsuleUnitManagerComponent::CompareUnit)
{
}

UCapsuleUnitManagerComponent::~UCapsuleUnitManagerComponent()
{
}

void UCapsuleUnitManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	m_UnitUpdater = MakeShared<UnitUpdater>(m_Material);
}


void UCapsuleUnitManagerComponent::UpdateUnits()
{
}


void UCapsuleUnitManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_UnitUpdater.IsValid())
	{
		m_UnitUpdater->UpdateUnit(DeltaTime, m_UpdateList, T_BeaconFire, m_Units.Num());
	}
}

void UCapsuleUnitManagerComponent::OnUnregister()
{
	for (auto unit: m_Units)
	{
		if (unit.Value)
		{
			unit.Value->UnregisterComponent();
			unit.Value->DestroyComponent();
		}
	}
	m_UpdateList.Empty();
	m_Units.Empty();

	Super::OnUnregister();
}




UUnitComponent* UCapsuleUnitManagerComponent::GetUnit(FVector index)
{
	return m_Units[index];
}


void UCapsuleUnitManagerComponent::AddToUpdateList(UUnitComponent* unit)
{
	if (unit && unit->GetManager() == this)
	{
		m_UpdateList.Push(unit);
	}
}


void UCapsuleUnitManagerComponent::TriggerUnit_Implementation(FVector index, float initValue)
{
	UUnitComponent* unit;
	if ((unit = GetUnit(index)) != nullptr)
	{
		unit->Value = initValue;
		unit->Trigger(T_BeaconFire);
		m_UpdateList.Push(unit);
	}
}

void UCapsuleUnitManagerComponent::TriggerUnit_Implementation(UUnitComponent* unit)
{
	unit->Trigger(T_BeaconFire);
	m_UpdateList.Push(unit);
}

void UCapsuleUnitManagerComponent::TriggerAllUnits_Implementation(float initValue)
{
	for (auto unit : m_Units)
	{
		unit.Value->Value = initValue;
		unit.Value->Trigger(T_BeaconFire);

		m_UpdateList.Push(unit.Value);
	}
}

void UCapsuleUnitManagerComponent::UnTriggerUnit_Implementation(FVector index, float value)
{
	UUnitComponent* unit;
	if ((unit = GetUnit(index)) != nullptr)
	{
		unit->Value = value;
		unit->UnTrigger();

		//no need to mannully remove the unit from Priority Queue
	}
}

void UCapsuleUnitManagerComponent::UnTriggerAllUnits_Implementation(float value)
{
	for (auto unit : m_Units)
	{
		unit.Value->Value = value;
		unit.Value->UnTrigger();

		m_UpdateList.Empty();
	}
}

void UCapsuleUnitManagerComponent::SetParameter(uint32 x)
{
	m_Count = x;
}

void UCapsuleUnitManagerComponent::SetParameter2(uint32 x, uint32 y)
{
	m_Count = x;
	m_Count_height = y;
}

void UCapsuleUnitManagerComponent::SetParameter3(uint32 x, uint32 y, uint32 z)
{
}

bool UCapsuleUnitManagerComponent::CompareUnit(UUnitComponent* a, UUnitComponent* b)
{
	return *a > *b;
}