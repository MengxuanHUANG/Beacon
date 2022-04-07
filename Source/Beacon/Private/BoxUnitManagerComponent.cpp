// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxUnitManagerComponent.h"
//Beacon Headers BEGIN
#include "BeaconMaterial.h"
#include "UnitUpdater.h"
#include "BeaconLog.h"
#include "LateUpdateComponent.h"
//Beacon Headers END

#include "DrawDebugHelpers.h"
#include "Curves/CurveFloat.h"
UBoxUnitManagerComponent::UBoxUnitManagerComponent()
	:m_UpdateList(UBoxUnitManagerComponent::CompareUnit)
{
}

void UBoxUnitManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	m_UnitUpdater = MakeShared<UnitUpdater>();


	ULateUpdateComponent* comp = ULateUpdateComponent::CreateLateUpdataComponent(this);
	comp->LateTickFunction.BindDynamic(this, &UBoxUnitManagerComponent::LateTickComponent);
	comp->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called every frame
void UBoxUnitManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_UnitUpdater.IsValid())
	{
		m_UnitUpdater->UpdateUnit(DeltaTime, m_UpdateList, T_BeaconFire, m_Units.Num());
		if (GetThermalProxyNeedUpdate())
		{
			UpdateThermalData();
			/*
			FlushPersistentDebugLines(GetOwner()->GetWorld());

			DrawDebugDirectionalArrow(
				GetOwner()->GetWorld(),
				GetComponentLocation(),
				GetComponentLocation() + ThermalData->Direction,
				100.f,
				FColor::Cyan,
				true,
				-1.f,
				0,
				5.0f
			);*/
		}
	}
}

void UBoxUnitManagerComponent::LateTickComponent(float DeltaTime)
{
	if (m_UnitUpdater.IsValid())
	{
		m_UnitUpdater->LateUpdateUnit(DeltaTime, m_UpdateList, m_Units.Num());
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
		unit->Value = m_Material->DefaultThermal;
		if (m_Material->Has_Max_BurningTime)
		{
			unit->SetMaxBurningTime(FMath::RandRange(m_Material->Min_BurningTime, m_Material->Max_BurningTime));
		}
	}
}

void UBoxUnitManagerComponent::SetReceiveThermalRadiation(bool enable)
{
	for (UUnitComponent* unit : m_Units)
	{
		unit->SetReceiveThermalRadiation(enable);
	}
}

bool UBoxUnitManagerComponent::CompareUnit(UUnitComponent* a, UUnitComponent* b)
{
	return *a > *b;
}

//BEGIN BeaconThermalProxy functions
void UBoxUnitManagerComponent::UpdateThermalData()
{
	if (ThermalData.IsValid())
	{
		float defaultThermal = m_Material->DefaultThermal;
		FVector dir(0);
		ThermalData->bIsBurning = m_UpdateList.Num() != 0;
		ThermalData->Thermal_Value = 0;

		float totalThermal = 0.f;
		if (ThermalData->bIsBurning)
		{
			for (UUnitComponent* unit : m_Units)
			{
				float unitThermal = unit->Value - defaultThermal;
				dir += unit->GetComponentLocation() * unitThermal;
				totalThermal += unitThermal;
			}

			ThermalData->Direction = totalThermal != 0 ? dir / totalThermal - GetComponentLocation(): dir;
		}
		ThermalData->Thermal_Value = totalThermal / float(m_Units.Num());
	}
}
//End BeaconThermalProxy functions