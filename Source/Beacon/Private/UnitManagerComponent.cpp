// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitManagerComponent.h"
#include "BeaconLog.h"

// Sets default values for this component's properties
UUnitManagerComponent::UUnitManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UUnitManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UUnitManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UUnitManagerComponent::OnUnregister()
{
	m_Material = nullptr;
	if (m_UnitUpdater.IsValid())
	{
		m_UnitUpdater.Reset();
	}
	Super::OnUnregister();
}