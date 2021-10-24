// Fill out your copyright notice in the Description page of Project Settings.


#include "FlammableComponent.h"
#include "BeaconCore.h"
#include "BeaconLog.h"

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
	:m_UnitCount(1, 1, 1)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetUsingAbsoluteScale(true);
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
				UCapsuleComponent* capsule = Cast<UCapsuleComponent>(component);
				capsule->OnComponentBeginOverlap.AddDynamic(this, &UFlammableComponent::OnBeginOverlap);

				CreateCapsuleFlammableUnits(capsule);
			}
			else if (name.Compare("SphereComponent") == 0)
			{
				USphereComponent* sphere = Cast<USphereComponent>(component);
				sphere->OnComponentBeginOverlap.AddDynamic(this, &UFlammableComponent::OnBeginOverlap);

				CreateSphereFlammableUnits(sphere);
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
	FVector extent = box->GetUnscaledBoxExtent();

	FVector size;
	size.X = extent.X / (float)m_UnitCount.X;
	size.Y = extent.Y / (float)m_UnitCount.Y;
	size.Z = extent.Z / (float)m_UnitCount.Z;

	//avoid cast data type in looping
	FVector count;
	count.X = (float)m_UnitCount.X;
	count.Y = (float)m_UnitCount.Y;
	count.Z = (float)m_UnitCount.Z;

	float count_x = (float)count.X / 2.0f;
	float count_y = (float)count.Y / 2.0f;
	float count_z = (float)count.Z / 2.0f;

	//allocate memory once for better perfomance
	m_FlammableUnits.Reserve(m_FlammableUnits.Num() + m_UnitCount.X * m_UnitCount.Y * m_UnitCount.Z);

	for (int x = 0; x < (int)m_UnitCount.X; x++)
	{
		for (int y = 0; y < (int)m_UnitCount.Y; y++)
		{
			for (int z = 0; z < (int)m_UnitCount.Z; z++)
			{
				UFlammableUnit* unit = NewObject<UFlammableUnit>(this);

				//register component for rendering
				unit->RegisterComponent();
				unit->Initialize(size, m_ConnectType);

				//setup attachment
				unit->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
				unit->SetRelativeLocation(
					FVector(
						2.0f * size.X * (x - count_x),
						2.0f * size.Y * (y - count_y),
						2.0f * size.Z * (z - count_z)
					) + size
				);
				unit->Ignite(T_FireParticle);
				m_FlammableUnits.Add(unit);
			}
		}
	}

	//TODO: remove
	//Manually ignite a unit
	if (m_InitializeWithFlame)
	{
		m_FlammableUnits[0]->Ignite(T_FireParticle);
		m_BurningUnits.Enqueue(m_FlammableUnits[0]);
	}

#ifdef BEACON_DEBUG
	for (UFlammableUnit* unit : m_FlammableUnits)
	{
		if (unit)
		{
			unit->DisplayDebugInfo();
		}
	}
#endif
}
void UFlammableComponent::CreateCapsuleFlammableUnits(const UCapsuleComponent* capsule)
{

}
void UFlammableComponent::CreateSphereFlammableUnits(const USphereComponent* sphere)
{
	float radius = sphere->GetUnscaledSphereRadius();
	float size = radius / ((float)m_Count - 0.5f) / FMath::Sqrt(3) / 2; // divided by sqrt(2)
	int count = ((int)m_Count);
	//int count = radius / m_UnitExtent.X;

	TArray<UFlammableUnit*> unitsArray;

#ifdef BEACON_DEBUG
	UE_LOG(LogTemp, Warning, TEXT("Count: %d"), count);
#endif

	int limit = FMath::Square(count + m_UnitExtent.X / count);

	for (int x = -count; x <= count; x++)
	{
		for (int y = -count; y <= count; y++)
		{
			for (int z = -count; z <= count; z++)
			{
				if (FMath::Abs(x) + FMath::Abs(y) + FMath::Abs(z) <= 3 * (count - 1))
				{
					UFlammableUnit* unit = NewObject<UFlammableUnit>(this);

					//register component for rendering
					unit->RegisterComponent();
					unit->Initialize(FVector(size), m_ConnectType);

					//setup attachment
					unit->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
					unit->SetRelativeLocation(
						FVector(
							2 * x * size,
							2 * y * size,
							2 * z * size
						)
					);
					unit->Ignite(T_FireParticle);
					unitsArray.Add(unit);
					m_FlammableUnitsMap.Add(FVector(x, y, z), unit);
				}
				else
				{
					unitsArray.Add(nullptr);
				}
			}
		}
	}
	
	unitsArray.Empty();

	if (m_InitializeWithFlame)
	{
		UFlammableUnit* unit = *(m_FlammableUnitsMap.Find(FVector(0, 0, 0)));
		unit->Ignite(T_FireParticle);
		m_BurningUnits.Enqueue(unit);
	}
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

	//TODO: move the "Unit manager"
	TQueue<UFlammableUnit*> temp;

	while (!m_BurningUnits.IsEmpty())
	{
		UFlammableUnit* unit;
		m_BurningUnits.Dequeue(unit);
		for (UFlammableUnit* neighbor : unit->GetNeighbors())
		{
			if (neighbor != nullptr && !(neighbor->IsBurning()))
			{
				neighbor->IncreaseTemperature(0.05f);
				if (neighbor->GetTemperature() >= 3.f)
				{
					neighbor->Ignite(T_FireParticle);
					temp.Enqueue(neighbor);
				}
				else
				{
					temp.Enqueue(unit);
				}
			}
		}
	}
	while (!temp.IsEmpty())
	{
		UFlammableUnit* unit;
		temp.Dequeue(unit);
		m_BurningUnits.Enqueue(unit);
	}
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