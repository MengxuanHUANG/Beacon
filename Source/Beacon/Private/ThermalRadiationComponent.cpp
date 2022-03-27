// Fill out your copyright notice in the Description page of Project Settings.


#include "ThermalRadiationComponent.h"
#include "BeaconThermalProxy.h"
#include "UnitManagerComponent.h"
#include "UnitComponent.h"
#include "BeaconLog.h"
#include "ThermalRadiationMaterial.h"

#include "Components/SphereComponent.h"

// Sets default values for this component's properties
UThermalRadiationComponent::UThermalRadiationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	RadiationSphere = CreateDefaultSubobject<USphereComponent>(FName("Radiation Sphere"));
	RadiationSphere->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
}

void UThermalRadiationComponent::DestroyComponent(bool bPromoteChildren)
{
	// ...
	if (RadiationSphere != nullptr)
	{
		RadiationSphere->DestroyComponent();
	}

	Super::DestroyComponent(bPromoteChildren);
}

// Called every frame
void UThermalRadiationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsEnabled)
	{
		UpdateLocation();
		FVector location = GetComponentLocation();

		for (UUnitComponent* unit : m_Units)
		{
			float distance = (unit->GetComponentLocation() - location).Size();
			if (!unit->IsTriggered())
			{
				float value = m_Material->GetThermalTransmitValue(distance);
				unit->AddValue(value);
			}
		}
	}
}

void UThermalRadiationComponent::Initialize(UThermalRadiationMaterial* material, bool enabled)
{
	bIsEnabled = enabled;
	m_Material = material;

	RadiationSphere->OnComponentBeginOverlap.AddDynamic(this, &UThermalRadiationComponent::OnBeginOverlap);
	RadiationSphere->OnComponentEndOverlap.AddDynamic(this, &UThermalRadiationComponent::OnEndOverlap);

	RadiationSphere->SetSphereRadius(m_Material->MAX_Radius, true);
}

void UThermalRadiationComponent::SetRadiationEnable(bool enable)
{
	bIsEnabled = enable;
}

void UThermalRadiationComponent::UpdateLocation()
{
	if (ThermalData.IsValid())
	{
		SetRelativeLocation(ThermalData->Direction, true);
	}
}

void UThermalRadiationComponent::OnBeginOverlap(UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName(BEACON_RECEIVE_THERMAL_RADIATION_TAG)))
	{
		UUnitComponent* other = Cast<UUnitComponent>(OtherComp->GetAttachParent());

		if (!m_Units.Contains(other) && !(other->IsTriggered()))
		{
			m_Units.Add(other);

			other->SetFlag(EUnitFlag::NeedUpdate);
			other->GetManager()->AddToUpdateList(other);
		}
	}
}

void UThermalRadiationComponent::OnEndOverlap(UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(FName(BEACON_RECEIVE_THERMAL_RADIATION_TAG)))
	{
		UUnitComponent* other = Cast<UUnitComponent>(OtherComp->GetAttachParent());

		m_Units.Remove(other);
	}
}