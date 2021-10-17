// Fill out your copyright notice in the Description page of Project Settings.


#include "FlammableComponent.h"
#include "BeaconCore.h"
#include "FlammableUnit.h"

#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"

#include "Components/StaticMeshComponent.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

#include "GameplayTagContainer.h"

#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UFlammableComponent::UFlammableComponent()
	:m_UnitExtent(30.f, 30.f, 30.f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//ignore parent's rotation
	SetUsingAbsoluteRotation(true);
}


// Called when the game starts
void UFlammableComponent::BeginPlay()
{
	Super::BeginPlay();

	const AActor* owner = GetOwner();

	if (owner != nullptr)
	{
		TArray<USceneComponent*> children;
		GetOwner()->GetRootComponent()->GetChildrenComponents(true, children);
		for (USceneComponent* component : children)
		{
			UClass* uClass = component->GetClass();
			FString name = uClass->GetFullGroupName(false);

			//Temp: Bind Dynamic Callback Function
			//TODO: template or abstract class
			if (name.Compare("BoxComponent") == 0)
			{
				UBoxComponent* box = Cast<UBoxComponent>(component);
				box->OnComponentBeginOverlap.AddDynamic(this, &UFlammableComponent::OnBeginOverlap);
				
				CreateBoxFlammableUnits(box);
			}
			else if (name.Compare("CapsuleComponent") == 0)
			{
				Cast<UCapsuleComponent>(component)->OnComponentBeginOverlap.AddDynamic(this, &UFlammableComponent::OnBeginOverlap);
			}
			else if (name.Compare("SphereComponent") == 0)
			{
				Cast<USphereComponent>(component)->OnComponentBeginOverlap.AddDynamic(this, &UFlammableComponent::OnBeginOverlap);
			}
		}
	}

	if (m_InitializeWithFlame)
	{
		//TODO: burn
	}
}

void UFlammableComponent::CreateBoxFlammableUnits(const UBoxComponent* box)
{
	//calculate number required to fit the box collider
	FVector extent = box->GetScaledBoxExtent();

	int count_x = extent.X / m_UnitExtent.X;
	int count_y = extent.Y / m_UnitExtent.Y;
	int count_z = extent.Z / m_UnitExtent.Z;

	//add offset if count is even
	FVector offset;
	offset.X = (~count_x & 1) * m_UnitExtent.X;
	offset.Y = (~count_y & 1) * m_UnitExtent.Y;
	offset.Z = (~count_z & 1) * m_UnitExtent.Z;

	//allocate memory once for better perfomance
	m_FlammableUnits.Reserve(m_FlammableUnits.Num() + count_x * count_y * count_z);
	
	for (int x = 0; x < count_x; x++)
	{
		for (int y = 0; y < count_y; y++)
		{
			for (int z = 0; z < count_z; z++)
			{
				UFlammableUnit* unit = NewObject<UFlammableUnit>(this);

				//register component for rendering
				unit->RegisterComponent();
				unit->Initialize(m_UnitExtent);

				//setup attachment
				unit->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
				unit->SetRelativeLocation(
					FVector(
						2 * (x - count_x / 2) * m_UnitExtent.X,
						2 * (y - count_y / 2) * m_UnitExtent.Y,
						2 * (z - count_z / 2) * m_UnitExtent.Z
					) + offset
				);
				unit->Ignite(T_FireParticle);
				m_FlammableUnits.Add(unit);
			}
		}
	}
}
void UFlammableComponent::CreateCapsuleFlammableUnits(const UCapsuleComponent* capsule)
{

}
void UFlammableComponent::CreateSphereFlammableUnits(const USphereComponent* sphere)
{

}

//Called when component destroyed
void UFlammableComponent::DestroyComponent(bool bPromoteChildren)
{
	for (UFlammableUnit* unit : m_FlammableUnits)
	{
		if (unit)
		{
			unit->DestroyComponent(bPromoteChildren);
		}
	}

	Super::DestroyComponent(bPromoteChildren);
}

// Called every frame
void UFlammableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFlammableComponent::Ignited(UParticleSystem* particle)
{
	//TODO: burn
}

void UFlammableComponent::OnBeginOverlap(UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!b_IsBurning && OtherActor->ActorHasTag(FName("Flammable")))
	{
#ifdef BEACON_DEBUG
		UE_LOG(LogTemp, Warning, TEXT("Ignite"));
#endif

		UFlammableComponent* otherflammable = Cast<UFlammableComponent>(OtherActor->GetComponentByClass(UFlammableComponent::StaticClass()));
		if (otherflammable->IsBurning())
		{
			this->Ignited(otherflammable->GetFireParticle());
		}
	}
}