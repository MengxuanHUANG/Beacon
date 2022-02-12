// Fill out your copyright notice in the Description page of Project Settings.


#include "DebrisUnitManagerComponent.h"

//Beacon Header Files
#include "BeaconLog.h"
#include "FractureComponent.h"
#include "FractureMaterial.h"
#include "BeaconMaterial.h"
#include "UnitUpdater.h"

UDebrisUnitManagerComponent::UDebrisUnitManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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
		//BEGIN Removed disabled debris in last update
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
		//END Removed disabled debris in last update

		//BEGIN Update flammableUnits information
		TArray<FFragment>& currentFragments = m_FractureComponent->GetCurrentFragments();
		TArray<FVector>& fragmentLocation = m_FractureComponent->GetFragmentsWorldLocation();
		TArray<FFragment>& needRemove = m_FractureComponent->GetNeedRemoveFragments();
		for (int i = 0; i < currentFragments.Num(); i++)
		{
			UFlammableUnitComponent* unit;
			const int32& index = currentFragments[i].Index;
			const int32& level = currentFragments[i].Level - 1;

			if (m_FlammableUnits.Contains(index))
			{
				unit = m_FlammableUnits[index];

				if (unit->CheckFlag(EUnitFlag::NeedUpdate))
				{
					//Update Location
					unit->SetWorldLocation(fragmentLocation[i], true);
				}
				else
				{
					unit->DestroyComponent();
					m_FlammableUnits.Remove(index);
					needRemove.Add(FFragment(index));
				}
			}
			else if(FMath::RandRange(0.f, 1.f) < m_FractureMaterial->FragmentLevels[level].Debris_BurningRate)
			{
				//Create New FlammableUnit
				unit = NewObject<UFlammableUnitComponent>(this);
				unit->RegisterComponent();

				unit->SetIndex(0);
				unit->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
				unit->SetWorldLocation(fragmentLocation[i]);

				unit->Initialize(nullptr, m_FractureMaterial->FragmentLevels[level].Debris_Size, ConnectType::None);
				unit->SetMaterial(m_UnitMaterials[level]);
				if (m_UnitMaterials[level]->Has_Max_BurningTime)
				{
					unit->SetMaxBurningTime(FMath::RandRange(m_UnitMaterials[level]->Min_BurningTime, m_UnitMaterials[level]->Max_BurningTime));
				}
				unit->Value = m_UnitMaterials[level]->DefaultThermal;
				unit->Trigger(m_FractureMaterial->FragmentLevels[level].T_BeaconFire);

				unit->Update(DeltaTime);

				m_FlammableUnits.Add(index, unit);
			}
			else
			{
				needRemove.Add(FFragment(index));
			}
		}
		//END Update flammableUnits information

		//BEGIN Update flammableUnits
		UpdateFlammableUnits(DeltaTime);
		//END Update flammableUnits
	}
}

void UDebrisUnitManagerComponent::UpdateFlammableUnits(float deltaTime)
{
	for (auto pair: m_FlammableUnits)
	{
		UFlammableUnitComponent* unit = pair.Value;
		unit->Update(deltaTime);

		//traverse all temp neighbors of a unit 
		TArray<TSharedPtr<UnitConnection>> tempConnections;
		unit->GetTemporaryNeighbors(tempConnections);
		for (TSharedPtr<UnitConnection>& connection : tempConnections)
		{
			if (!connection->Other->IsTriggered())
			{
				connection->Other->Value += deltaTime;
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
		UBeaconMaterial* material = DuplicateObject<UBeaconMaterial>(level.BeaconMaterial, this);

		m_UnitMaterials.Add(material);
		material = nullptr;
	}
}

//Handle fracture event
void UDebrisUnitManagerComponent::OnFractured(bool needUpdate)
{
	bIsFractured = needUpdate;
}