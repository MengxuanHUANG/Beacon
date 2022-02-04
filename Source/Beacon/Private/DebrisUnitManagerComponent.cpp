// Fill out your copyright notice in the Description page of Project Settings.


#include "DebrisUnitManagerComponent.h"

//Beacon Header Files
#include "BeaconLog.h"
#include "FractureComponent.h"
#include "FractureMaterial.h"
#include "BeaconMaterial.h"

UDebrisUnitManagerComponent::UDebrisUnitManagerComponent()
{
}

UDebrisUnitManagerComponent::~UDebrisUnitManagerComponent()
{
	m_FractureComponent = nullptr;
}

// Called when the game starts
void UDebrisUnitManagerComponent::BeginPlay() 
{
	Super::BeginPlay();
}

void UDebrisUnitManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsFractured)
	{
		//removed disabled debris in last update
		TArray<FFragment>& removdFragments = m_FractureComponent->GetRemovedFragments();
		for (const FFragment& fragment : removdFragments)
		{
			if (m_FlammableUnits.Contains(fragment.Index))
			{
				UFlammableUnitComponent* unit = m_FlammableUnits[fragment.Index];
				unit->DestroyComponent();
				m_FlammableUnits.Remove(fragment.Index);
			}
		}

		TArray<FFragment>& currentFragments = m_FractureComponent->GetCurrentFragments();
		TArray<FVector>& fragmentLocation = m_FractureComponent->GetFragmentsWorldLocation();
		TArray<FFragment>& needRemove = m_FractureComponent->GetNeedRemoveFragments();
		for (int i = 0; i < currentFragments.Num(); i++)
		{
			UFlammableUnitComponent* unit;
			const int32& index = currentFragments[i].Index;
			const int32& level = currentFragments[i].Level;

			if (m_FlammableUnits.Contains(index))
			{
				unit = m_FlammableUnits[index];
				//Update Location
				unit->SetWorldLocation(fragmentLocation[i], true);
			}
			else if(FMath::RandRange(0.f, 1.f) < m_FractureMaterial->FragmentLevels[level].Debris_BurningRate)
			{
				//Create New FlammableUnit
				unit = NewObject<UFlammableUnitComponent>(this);
				unit->RegisterComponent();

				unit->SetIndex(0);
				unit->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
				unit->SetWorldLocation(fragmentLocation[i]);

				unit->Initialize(this, m_FractureMaterial->FragmentLevels[level].Debris_Size, ConnectType::None);
				unit->SetMaterial(m_UnitMaterials[level]);
				unit->Trigger(m_FractureMaterial->FragmentLevels[level].T_BeaconFire);

				m_FlammableUnits.Add(index, unit);
			}
			else
			{
				needRemove.Add(FFragment(index));
			}
		}
	}
}

void UDebrisUnitManagerComponent::OnUnregister()
{
	//Remove callback function if fractureComponent exist
	if (m_FractureComponent)
	{
		m_FractureComponent->OnFracturedEvent.RemoveDynamic(this, &UDebrisUnitManagerComponent::OnFractured);
	}
	m_FractureComponent = nullptr;
	Super::OnUnregister();
}

void UDebrisUnitManagerComponent::Initialize(UFractureComponent* fractureComponent, bool isFractured)
{
	m_FractureComponent = fractureComponent;
	//Bind callback function if fractureComponent exist
	if (!isFractured && m_FractureComponent)
	{
		m_FractureComponent->OnFracturedEvent.AddDynamic(this, &UDebrisUnitManagerComponent::OnFractured);
	}
}

void UDebrisUnitManagerComponent::SetFractureMaterial(UFractureMaterial* fractureMaterial)
{
	m_FractureMaterial = fractureMaterial;
	for (const FFragmentLevel& level : m_FractureMaterial->FragmentLevels)
	{
		UBeaconMaterial* material = NewObject<UBeaconMaterial>(this);
		material->Has_Max_BurningTime = level.Has_Max_BurningTime;
		material->Flash_Point = level.Flash_Point;
		if (material->Has_Max_BurningTime)
		{
			material->Max_BurningTime = FMath::RandRange(level.Min_BurningTime, level.Max_BurningTime);
		}

		m_UnitMaterials.Add(material);
		material = nullptr;
	}
}

//Handle fracture event
void UDebrisUnitManagerComponent::OnFractured()
{
	bIsFractured = true;
}