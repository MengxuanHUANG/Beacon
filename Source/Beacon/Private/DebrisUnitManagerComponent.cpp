// Fill out your copyright notice in the Description page of Project Settings.


#include "DebrisUnitManagerComponent.h"

//Beacon Header Files
#include "FractureComponent.h"
#include "BeaconLog.h"

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
			UFlammableUnitComponent* unit = m_FlammableUnits[fragment.Index];
			if (unit)
			{
				unit->DestroyComponent();
				m_FlammableUnits.Remove(fragment.Index);
			}
		}

		TArray<FFragment>& currentFragments = m_FractureComponent->GetCurrentFragments();
		TArray<FVector>& fragmentLocation = m_FractureComponent->GetFragmentsWorldLocation();
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
			else
			{
				//Create New FlammableUnit
				unit = NewObject<UFlammableUnitComponent>(this);
				unit->RegisterComponent();

				unit->SetIndex(0);
				unit->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
				unit->SetWorldLocation(fragmentLocation[i]);

				unit->Initialize(this, FVector(10), ConnectType::None);

				m_FlammableUnits.Add(index, unit);
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

void UDebrisUnitManagerComponent::Initialize(UFractureComponent* fractureComponent, int32 rootIndex, bool isFractured)
{
	m_FlammableUnits.Add(rootIndex, nullptr);
	m_FractureComponent = fractureComponent;
	//Bind callback function if fractureComponent exist
	if (!isFractured && m_FractureComponent)
	{
		m_FractureComponent->OnFracturedEvent.AddDynamic(this, &UDebrisUnitManagerComponent::OnFractured);
	}
}

//Handle fracture event
void UDebrisUnitManagerComponent::OnFractured()
{
	bIsFractured = true;
}