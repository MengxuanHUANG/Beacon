// Fill out your copyright notice in the Description page of Project Settings.


#include "FractureComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "PhysicsProxy/GeometryCollectionPhysicsProxy.h"

#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"

//Beacon Header files
#include "BeaconLog.h"
#include "DebrisUnitManagerComponent.h"
#include "FractureMaterial.h"
#include "Field/FieldSystemActor.h"

UFractureComponent::UFractureComponent()
	:bIsBreak(false), bNeedUpdate(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//Create GeometryCollectionComponent
	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(FName("Geometry Collection"));
	GeometryCollectionComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	m_DebrisUnitManager = CreateDefaultSubobject<UDebrisUnitManagerComponent>(FName("Debris Unit Manager"));
	m_DebrisUnitManager->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts
void UFractureComponent::BeginPlay()
{
	Super::BeginPlay();

	bIsBreak = false;
	m_DebrisUnitManager->Initialize(this, bIsBreak);
	m_DebrisUnitManager->SetFractureMaterial(T_FractureMaterial);
}

// Called every frame
void UFractureComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsBreak && bNeedUpdate)
	{
		//Update Current fragments state (location, wehther disabled)
		UpdateCurrentDebris();
	}
	else if(!bIsBreak)
	{
		if (m_ThermalData.IsValid() && m_ThermalData->Thermal_Value >= m_BreakThermal)
		{
			Break();
		}
		bIsBreak = CheckBreak();
		if (bIsBreak)
		{
			if (m_ThermalData.IsValid() && m_ThermalData->bIsBurning)
			{
				bNeedUpdate = m_ThermalData->bIsBurning;
				//Update Current fragments state (location, wehther disabled)
				UpdateCurrentDebris();
			}

			OnFracturedEvent.Broadcast(bNeedUpdate);
		}
	}
}

bool UFractureComponent::InitializeCurrentDebris()
{
	//Clear CurrentDebrisIndex
	m_CurrentFragments.Empty();

	//Obtain Root node's index
	if (GeometryCollectionComponent->RestCollection)
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
		BEACON_LOG_FULL(Error, "RestCollection is required for GeometryCollectionComponent!");
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
			//Remove Uncessary fragments
			for (FFragment& fragment : m_NeedRemoveFragments)
			{
				m_CurrentFragments.Remove(fragment);
			}
			m_NeedRemoveFragments.Empty();

			//BEGIN update CurrentDebrisIndex
			const TManagedArray<TSet<int32> >& childrenArr = GeometryCollectionComponent->GetChildrenArray();

			m_RemovedFragments.Empty();
			TArray<FFragment> markedEmplace;
			for (FFragment& fragment : m_CurrentFragments)
			{
				const int32& index = fragment.Index;
				const int32& level = fragment.Level;

				if (disabled[index])
				{
					//mark index to be removed
					m_RemovedFragments.Emplace(FFragment(index, level));

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

			for (FFragment& fragment : m_RemovedFragments)
			{
				m_CurrentFragments.RemoveSwap(fragment);
			}
			for (FFragment& fragment : markedEmplace)
			{
				m_CurrentFragments.Emplace(fragment);
			}
			//END update CurrentDebrisIndex
			
			//BEGIN access debris' world location
			m_FragmentWorldLocation.Empty();
			TManagedArray<TUniquePtr<Chaos::FGeometryParticle>>& particles = physicsProxy->GetExternalParticles();

			for (FFragment& fragment : m_CurrentFragments)
			{
				TUniquePtr<Chaos::FGeometryParticle>& particle = particles[fragment.Index];
				FVector location = particle->X();
				m_FragmentWorldLocation.Add(location);
			}
			//END access debris' world location
		}
	}
}

void UFractureComponent::ClearCurrentDebris()
{
	//Clear CurrentDebrisIndex
	m_CurrentFragments.Empty();
}

bool UFractureComponent::BeaconCheck_Implementation(FString& Info, FString& Message)
{
	BEACON_CLEAR_FSTRING(Info);
	BEACON_CLEAR_FSTRING(Message);

	BEACON_AUTO_FILL_INFO(Info);

	FString errors;
	if (!bIsBuilded)
	{
		errors.Append("Error: FractureComponent has not been built! Please use BeaconEditor to build it!\n");
	}
	if (T_FractureMaterial == nullptr)
	{
		errors.Append("Error: FractureMaterial is Reuqired!\n");
	}
	if (GeometryCollectionComponent->RestCollection == nullptr)
	{
		errors.Append("Error: RestCollection in GeometryCollectionComponent must be set!\n");
	}

	if (errors.IsEmpty())
	{
		FString name = GetOwner()->GetName();
		Message.Append(name);
		Message.Append(": FractureComponent pass runtime check.");
		return true;
	}
	else
	{
		Message.Append(errors);
		return false;
	}
}

//Begin implementing BuildableComponent functions
bool UFractureComponent::Build_Implement()
{
	bool result =  this->InitializeCurrentDebris();

#ifdef BEACON_DEBUG
	if (!result)
	{
		BEACON_LOG(Warning, "UFractureComponent build failed!");
	}
#endif

	return result;
}
void UFractureComponent::Clear_Implement()
{
	this->ClearCurrentDebris();
}
//End implementing BuildableComponent functions

void UFractureComponent::Break_Implementation()
{
	FVector location = GetComponentLocation();
	
	AFieldSystemActor* breakField = GetWorld()->SpawnActor<AFieldSystemActor>(T_BreakField, location, FRotator::ZeroRotator);
}

bool UFractureComponent::CheckBreak()
{
	//obtain PhysicProxy
	FGeometryCollectionPhysicsProxy* physicsProxy = GeometryCollectionComponent->GetPhysicsProxy();

	//const FGeometryCollectionResults* results = physicsProxy ? physicsProxy->GetConsumerResultsGT() : nullptr;

	const TArray<bool>& disabled = GeometryCollectionComponent->GetDisabledFlags();

	if (disabled.Num() > m_CurrentFragments[0].Index)
	{
		return disabled[m_CurrentFragments[0].Index];
	}
	return false;
}