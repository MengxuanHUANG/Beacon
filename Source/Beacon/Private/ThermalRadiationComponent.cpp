// Fill out your copyright notice in the Description page of Project Settings.


#include "ThermalRadiationComponent.h"
#include "ThermalRadiationMaterial.h"
#include "BeaconThermalProxy.h"
#include "UnitComponent.h"
#include "BeaconLog.h"

#include "Components/SphereComponent.h"

// Sets default values for this component's properties
UThermalRadiationComponent::UThermalRadiationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	RadiationSphere = CreateDefaultSubobject<USphereComponent>(FName("Radiation Sphere"));
	RadiationSphere->AttachTo(this);
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

void UThermalRadiationComponent::Initialize(UThermalRadiationMaterial* material, bool enabled)
{
	bIsEnabled = enabled;
	m_Material = material;

	RadiationSphere->OnComponentBeginOverlap.AddDynamic(this, &UThermalRadiationComponent::OnBeginOverlap);
	RadiationSphere->OnComponentEndOverlap.AddDynamic(this, &UThermalRadiationComponent::OnEndOverlap);

	RadiationSphere->SetSphereRadius(m_Material->MAX_Radius, true);
}

// Called every frame
void UThermalRadiationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsEnabled)
	{
		for (auto pair : m_Units)
		{
			UUnitComponent* unit = pair.Value;

			//TODO: based on material and thermal data
			unit->Value += 10.f;
		}
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

		if (!(other->IsTriggered()))
		{
			FString actorName;
			FString compName;
			OtherActor->GetName(actorName);
			other->GetName(compName);

			FString id = actorName.Append("_" + compName);

			if (!m_Units.Contains(id))
			{
				FVector dir = other->GetComponentLocation() - GetComponentLocation();
				m_Units.Add(id, other);

				other->SetFlag(EUnitFlag::NeedUpdate);
				other->GetManager()->AddToUpdateList(other);
			}
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

		FString actorName;
		FString compName;
		OtherActor->GetName(actorName);
		other->GetName(compName);

		FString id = actorName.Append("_" + compName);

		m_Units.Remove(id);
	}
}