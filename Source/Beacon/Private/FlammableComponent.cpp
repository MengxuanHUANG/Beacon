// Fill out your copyright notice in the Description page of Project Settings.


#include "FlammableComponent.h"
#include "BeaconCore.h"

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
				unit->Initialize(m_UnitExtent, m_ConnectType);

				//setup attachment
				unit->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
				unit->SetRelativeLocation(
					FVector(
						2 * (x - count_x / 2) * m_UnitExtent.X,
						2 * (y - count_y / 2) * m_UnitExtent.Y,
						2 * (z - count_z / 2) * m_UnitExtent.Z
					) + offset
				);

				//bind pointer
				if (m_ConnectType == ConnectType::SixDirection)
				{
					if (x - 1 >= 0)
					{
						unit->SetNeighbor(-1, 0, 0, m_FlammableUnits[(x - 1) * count_y * count_z + y * count_z + z]);
					}
					if (y - 1 >= 0)
					{
						unit->SetNeighbor(0, -1, 0, m_FlammableUnits[x * count_y * count_z + (y - 1) * count_z + z]);
					}
					if (z - 1 >= 0)
					{
						unit->SetNeighbor(0, 0, -1, m_FlammableUnits[x * count_y * count_z + y * count_z + z - 1]);
					}
				}
				//unit->Ignite(T_FireParticle);
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
	int count = radius / m_UnitExtent.X;

#ifdef BEACON_DEBUG
	UE_LOG(LogTemp, Warning, TEXT("Count: %d"), count);
#endif

	int limit = FMath::Square(count + m_UnitExtent.X / count);

	for (int x = -(count >> 1); x <= (count >> 1); x++)
	{
		for (int y = -(count >> 1); y <= (count >> 1); y++)
		{
			for (int z = -(count >> 1); z <= (count >> 1); z++)
			{
				if (((x*x + y*y + z*z) << 2) <= FMath::Square(count))
				{
					UFlammableUnit* unit = NewObject<UFlammableUnit>(this);

					//register component for rendering
					unit->RegisterComponent();
					unit->Initialize(m_UnitExtent, m_ConnectType);

					//setup attachment
					unit->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
					unit->SetRelativeLocation(
						FVector(
							2 * x * m_UnitExtent.X,
							2 * y * m_UnitExtent.Y,
							2 * z * m_UnitExtent.Z
						)
					);

					//bind pointer
					if (m_ConnectType == ConnectType::SixDirection)
					{
						UFlammableUnit** xp = m_FlammableUnitsMap.Find(FVector(x - 1, y, z));
						UFlammableUnit** yp = m_FlammableUnitsMap.Find(FVector(x, y - 1, z));
						UFlammableUnit** zp = m_FlammableUnitsMap.Find(FVector(x, y, z - 1));
						if (xp != nullptr)
						{
							unit->SetNeighbor(-1, 0, 0, *xp);
						}
						if (yp != nullptr)
						{
							unit->SetNeighbor(0, -1, 0, *yp);
						}
						if (zp != nullptr)
						{
							unit->SetNeighbor(0, 0, -1, *zp);
						}
					}

					m_FlammableUnitsMap.Add(FVector(x, y, z), unit);
				}
			}
		}
	}

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