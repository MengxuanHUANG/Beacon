// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildableComponent.h"
#include "BeaconCore.h"

// Sets default values for this component's properties
UBuildableComponent::UBuildableComponent()
	:bIsBuilded(false)
{
	AActor* owner = GetOwner();
	if (owner != nullptr && !(owner->Tags.Contains(FName(BEACON_BUILDABLE_TAG))))
	{
		owner->Tags.Emplace(FName(BEACON_BUILDABLE_TAG));
	}
}

void UBuildableComponent::DestroyComponent(bool bPromoteChildren)
{
	AActor* owner = GetOwner();
	if (owner != nullptr)
	{
		owner->Tags.RemoveSwap(FName(BEACON_BUILDABLE_TAG));
	}

	Super::DestroyComponent(bPromoteChildren);
}

// Called when the game starts
void UBuildableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UBuildableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBuildableComponent::Clear()
{
	if (bIsBuilded)
	{
		Clear_Implement();
		bIsBuilded = false;
	}
}

void UBuildableComponent::Build()
{
	if (!bIsBuilded)
	{
		bIsBuilded = Build_Implement();
	}
}

void UBuildableComponent::ReBuild()
{
	Clear();
	bIsBuilded = Build_Implement();
}