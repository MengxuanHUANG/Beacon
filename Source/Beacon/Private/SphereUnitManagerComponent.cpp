// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereUnitManagerComponent.h"
#include "UnitComponent.h"
#include "BeaconLog.h"
#include "UnitUpdater.h"

USphereUnitManagerComponent::USphereUnitManagerComponent()
	:m_UpdateList(USphereUnitManagerComponent::CompareUnits)
{
}

USphereUnitManagerComponent::~USphereUnitManagerComponent()
{
}

void USphereUnitManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	m_UnitUpdater = MakeShared<UnitUpdater>();
}

// Called every frame
void USphereUnitManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (m_UnitUpdater.IsValid())
	{
		m_UnitUpdater->UpdateUnit(DeltaTime, m_UpdateList, T_BeaconFire, m_Units.Num());
	}
}

void USphereUnitManagerComponent::OnUnregister()
{
	for (auto unit : m_Units)
	{
		if (unit.Value)
		{
			unit.Value->DestroyComponent();
		}
	}
	m_UpdateList.Empty();
	m_Units.Empty();

	Super::OnUnregister();
}

void USphereUnitManagerComponent::SetParameter(uint32 x)
{
	m_Count = x;
}

UUnitComponent* USphereUnitManagerComponent::GetUnit(FVector index)
{
	if (!m_Units.Contains(index))
	{
		return nullptr;
	}
	else
	{
		return m_Units[index];
	}
}

void USphereUnitManagerComponent::AddToUpdateList(UUnitComponent* unit)
{
	if (unit && unit->GetManager() == this)
	{
		m_UpdateList.Push(unit);
	}
}

void USphereUnitManagerComponent::TriggerUnit_Implementation(FVector index, float initValue)
{
	UUnitComponent* unit;
	if ((unit = GetUnit(index)) != nullptr)
	{
		unit->Value = initValue;
		unit->Trigger(T_BeaconFire);
		m_UpdateList.Push(unit);
	}
}

void USphereUnitManagerComponent::TriggerUnit_Implementation(UUnitComponent* unit)
{
	if (unit && unit->GetManager() == this)
	{
		unit->Trigger(T_BeaconFire);
		m_UpdateList.Push(unit);
	}
}

void USphereUnitManagerComponent::TriggerAllUnits_Implementation(float initValue)
{
	for (auto unit : m_Units)
	{
		unit.Value->Value = initValue;
		unit.Value->Trigger(T_BeaconFire);

		m_UpdateList.Push(unit.Value);
	}
}

void USphereUnitManagerComponent::UnTriggerUnit_Implementation(FVector index, float value)
{
	UUnitComponent* unit;
	if ((unit = GetUnit(index)) != nullptr)
	{
		unit->Value = value;
		unit->UnTrigger();

		//no need to mannully remove the unit from Priority Queue
	}
}

void USphereUnitManagerComponent::UnTriggerAllUnits_Implementation(float value)
{
	for (auto unit : m_Units)
	{
		unit.Value->Value = value;
		unit.Value->UnTrigger();

		m_UpdateList.Empty();
	}
}

void USphereUnitManagerComponent::SetUnitsMaterial()
{
	for (auto unit : m_Units)
	{
		unit.Value->SetMaterial(m_Material);
		unit.Value->Value = m_Material->DefaultThermal;
		if (m_Material->Has_Max_BurningTime)
		{
			unit.Value->SetMaxBurningTime(FMath::RandRange(m_Material->Min_BurningTime, m_Material->Max_BurningTime));
		}
	}
}

bool USphereUnitManagerComponent::CompareUnits(UUnitComponent* a, UUnitComponent* b)
{
	return *a > *b;
}