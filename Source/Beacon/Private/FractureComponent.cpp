// Fill out your copyright notice in the Description page of Project Settings.


#include "FractureComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "PhysicsProxy/GeometryCollectionPhysicsProxy.h"

#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"

//Beacon Header files
#include "BeaconLog.h"

UFractureComponent::UFractureComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//Create GeometryCollectionComponent
	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(FName("Geometry Collection"));
	GeometryCollectionComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts
void UFractureComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UFractureComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Update Current fragments state (location, wehther disabled)
	UpdateCurrentDebris();

	FGeometryCollectionPhysicsProxy* physicsProxy = GeometryCollectionComponent ? GeometryCollectionComponent->GetPhysicsProxy() : nullptr;
	TManagedArray<TUniquePtr<Chaos::FGeometryParticle>>& particles = physicsProxy->GetExternalParticles();

	//BEGIN update flames location
	for (const FFragment& fragment : m_CurrentFragments)
	{
		const int32& index = fragment.Index;
		const int32& level = fragment.Level;

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

bool UFractureComponent::InitializeCurrentDebris()
{
	//Clear CurrentDebrisIndex
	m_CurrentFragments.Empty();

	//Obtain Root node's index
	if (GeometryCollectionComponent)
	{
		const TManagedArray<int32>& parentArr = GeometryCollectionComponent->GetParentArray();
		for (int index = 0; index < parentArr.Num(); ++index)
		{
			if (parentArr[index] == -1)
			{
				m_CurrentFragments.Add(FFragment(index, 0));
				break;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

void UFractureComponent::UpdateCurrentDebris()
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

			TArray<FFragment> markedRemove;
			TArray<FFragment> markedEmplace;

			for (FFragment& fragment : m_CurrentFragments)
			{
				const int32& index = fragment.Index;
				const int32& level = fragment.Level;

				if (disabled[index])
				{
					//TODO: remove
					if (index < m_Boxes.Num() && m_Boxes[index] != nullptr)
					{
						m_Boxes[index]->UnregisterComponent();
						m_Boxes[index] = nullptr;
					}
					//End TODO

					//mark index to be removed
					markedRemove.Emplace(FFragment(index, level));

					//mark new debris to be emplace
					TQueue<FFragment> temp;
					temp.Enqueue(FFragment(index, level)); 
					FFragment parent;
					while (temp.Dequeue(parent))
					{
						for (auto childIndex : childrenArr[index])
						{
							if (disabled[childIndex])
							{
								temp.Enqueue(FFragment(childIndex, parent.Level + 1));
							}
							else
							{
								markedEmplace.Emplace(FFragment(childIndex, parent.Level + 1));
							}
						}
					}
				}
			}

			for (FFragment& fragment : markedRemove)
			{
				m_CurrentFragments.RemoveSwap(fragment);
			}
			for (FFragment& fragment : markedEmplace)
			{
				m_CurrentFragments.Emplace(fragment);
			}
			//END update CurrentDebrisIndex
		}
	}
}

void UFractureComponent::ClearCurrentDebris()
{
	//Clear CurrentDebrisIndex
	m_CurrentFragments.Empty();
}

//Begin implementing BuildableComponent functions
bool UFractureComponent::Build_Implement()
{
	bool result =  this->InitializeCurrentDebris();
	m_Boxes.Init(nullptr, GeometryCollectionComponent->RestCollection->GetGeometryCollection()->TransformToGeometryIndex.Num());

#ifdef BEACON_DEBUG
	if (!result)
	{
		BEACON_LOG_FULL(Display, "Build Error");
	}
#endif

	return result;
}
void UFractureComponent::Clear_Implement()
{
	this->ClearCurrentDebris();
}
//End implementing BuildableComponent functions