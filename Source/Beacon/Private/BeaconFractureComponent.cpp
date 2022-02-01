// Fill out your copyright notice in the Description page of Project Settings.


#include "BeaconFractureComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "PhysicsProxy/GeometryCollectionPhysicsProxy.h"

#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"

//Beacon Header files
#include "BeaconLog.h"

// Sets default values for this component's properties
UBeaconFractureComponent::UBeaconFractureComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(FName("Geometry Collection"));
	GeometryCollectionComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts
void UBeaconFractureComponent::BeginPlay()
{
	Super::BeginPlay();

	this->InitializeCurrentDebris();
	
	m_Boxes.Init(nullptr, GeometryCollectionComponent->RestCollection->GetGeometryCollection()->TransformToGeometryIndex.Num());
}

// Called every frame
void UBeaconFractureComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	this->UpdateCurrentDebris();

	FGeometryCollectionPhysicsProxy* physicsProxy = GeometryCollectionComponent ? GeometryCollectionComponent->GetPhysicsProxy() : nullptr;
	TManagedArray<TUniquePtr<Chaos::FGeometryParticle>>& particles = physicsProxy->GetExternalParticles();

	//BEGIN update flames location
	for (TPair<int32, int32>& pair : m_CurrentDebrisIndex)
	{
		const int32& index = pair.Key;
		const int32& level = pair.Value;

		TUniquePtr<Chaos::FGeometryParticle>& particle = particles[index];
		FVector location = particle->X();
		

		if (m_Boxes[index] == nullptr)
		{
			UBoxComponent* box = NewObject<UBoxComponent>(this);
			box->RegisterComponent();
			box->SetBoxExtent(FVector(size));
			box->bHiddenInGame = false;
			box->ShapeColor = color;

			m_Boxes[index] = box;
		}
		m_Boxes[index]->SetWorldLocation(location);
	}
	//END update
}

void UBeaconFractureComponent::InitializeCurrentDebris()
{
	//Clear CurrentDebrisIndex
	m_CurrentDebrisIndex.Empty();

	//Obtain Root node's index
	if (GeometryCollectionComponent)
	{
		const TManagedArray<int32>& parentArr = GeometryCollectionComponent->GetParentArray();
		for (int index = 0; index < parentArr.Num(); ++index)
		{
			if (parentArr[index] == -1)
			{
				m_CurrentDebrisIndex.Add(TPair<int32, int32>(index, 0));
				break;
			}
		}
	}
}

void UBeaconFractureComponent::UpdateCurrentDebris()
{
	//obtain PhysicProxy
	FGeometryCollectionPhysicsProxy* physicsProxy = GeometryCollectionComponent ? GeometryCollectionComponent->GetPhysicsProxy() : nullptr;

	//const FGeometryCollectionResults* results = physicsProxy ? physicsProxy->GetConsumerResultsGT() : nullptr;
	if (physicsProxy)
	{
		const TArray<bool>& disabled = GeometryCollectionComponent->GetDisabledFlags();

		if (disabled.Num() > 0)
		{
			//BEGIN update CurrentDebrisIndex
			const TManagedArray<TSet<int32> >& childrenArr = GeometryCollectionComponent->GetChildrenArray();

			TArray<TPair<int32, int32>> markedRemove;
			TArray<TPair<int32, int32>> markedEmplace;

			for (TPair<int32, int32>& pair : m_CurrentDebrisIndex)
			{
				const int32& index = pair.Key;
				const int32& level = pair.Value;

				if (disabled[index])
				{
					//mark index to be removed
					markedRemove.Emplace(TPair<int32, int32>(index, level));

					//mark new debris to be emplace
					TQueue<TPair<int32, int32>> temp;
					temp.Enqueue(TPair<int32, int32>(index, level));
					TPair<int32, int32> parent;
					while (temp.Dequeue(parent))
					{
						for (auto childIndex : childrenArr[index])
						{
							if (disabled[childIndex])
							{
								temp.Enqueue(TPair<int32, int32>(childIndex, parent.Value + 1));
							}
							else
							{
								markedEmplace.Emplace(TPair<int32, int32>(childIndex, parent.Value + 1));
							}
						}
					}
				}
			}

			for (TPair<int32, int32>& pair : markedRemove)
			{
				m_CurrentDebrisIndex.RemoveSwap(pair);
			}
			for (TPair<int32, int32>& pair : markedEmplace)
			{
				m_CurrentDebrisIndex.Emplace(pair);
			}
			//END update CurrentDebrisIndex
		}
	}
}