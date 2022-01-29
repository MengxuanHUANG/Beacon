// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UnitComponent.h"
#include "Beacon_PriorityQueue.h"

#include "UnitManagerComponent.h"
#include "Components/BoxComponent.h"

#include "BeaconLog.h"

#include "BoxUnitManagerComponent.generated.h"

class UBeaconFire;

/**
 * 
 */
UCLASS()
class BEACON_API UBoxUnitManagerComponent : public UUnitManagerComponent
{
	GENERATED_BODY()
public:
	UBoxUnitManagerComponent();
	~UBoxUnitManagerComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnUnregister() override;
	
	virtual void UpdateUnits() override;
	virtual UUnitComponent* GetUnit(FVector index) override;

	virtual void AddToUpdateList(UUnitComponent* unit) override;
	virtual void TriggerUnit_Implementation(UUnitComponent* unit) override;
	virtual void TriggerUnit_Implementation(FVector index, float initValue) override;
	virtual void TriggerAllUnits_Implementation(float initValue) override;
	virtual void UnTriggerUnit_Implementation(FVector index, float value) override;
	virtual void UnTriggerAllUnits_Implementation(float value) override;
	
	virtual void SetParameter3(uint32 x, uint32 y, uint32 z) override;

	virtual FVector LocalLocation2Index(FVector location) const override;

private:
	static bool CompareUnit(UUnitComponent* a, UUnitComponent* b);

public:
	float last;

	UPROPERTY(VisibleAnywhere)
		TArray<UUnitComponent*> m_Units;
	
	UPROPERTY(VisibleAnywhere)
		FVector m_UnitCount;
	
	Beacon_PriorityQueue<UUnitComponent> m_UpdateList;

	UPROPERTY(VisibleAnywhere)
		const UBoxComponent* ParentBox;

public:
	template<typename InnerUnitType, typename OuterUnitType>
	static void CreateUnit(UBoxUnitManagerComponent* boxUnitManager, USceneComponent* self, const USceneComponent* parent)
	{
		const UBoxComponent* box = Cast<UBoxComponent>(parent);
		boxUnitManager->ParentBox = box;

		//calculate number required to fit the box collider
		FVector extent = box->GetUnscaledBoxExtent();

		FVector size;
		size.X = extent.X / (float)(boxUnitManager->m_UnitCount.X);
		size.Y = extent.Y / (float)(boxUnitManager->m_UnitCount.Y);
		size.Z = extent.Z / (float)(boxUnitManager->m_UnitCount.Z);

		//avoid cast data type in looping
		FVector count;
		count.X = (float)(boxUnitManager->m_UnitCount.X);
		count.Y = (float)(boxUnitManager->m_UnitCount.Y);
		count.Z = (float)(boxUnitManager->m_UnitCount.Z);

		float count_x = (float)count.X / 2.0;
		float count_y = (float)count.Y / 2.0;
		float count_z = (float)count.Z / 2.0;

		//allocate memory once for better perfomance
		(boxUnitManager->m_Units).Reserve(
			(boxUnitManager->m_Units).Num() +
			boxUnitManager->m_UnitCount.X *
			boxUnitManager->m_UnitCount.Y *
			boxUnitManager->m_UnitCount.Z);

		for (int x = 0; x < (int)(boxUnitManager->m_UnitCount.X); x++)
		{
			for (int y = 0; y < (int)(boxUnitManager->m_UnitCount.Y); y++)
			{
				for (int z = 0; z < (int)(boxUnitManager->m_UnitCount.Z); z++)
				{
					UUnitComponent* unit;
					if (x == 0 || y == 0 || z == 0 
						|| x == boxUnitManager->m_UnitCount.X - 1
						|| y == boxUnitManager->m_UnitCount.Y - 1
						|| z == boxUnitManager->m_UnitCount.Z - 1)
					{
						unit = NewObject<OuterUnitType>(self);
					}
					else
					{
						unit = NewObject<InnerUnitType>(self);
					}
					//register component for rendering
					unit->RegisterComponent();
					unit->SetIndex(x * count.Y * count.Z + y * count.Z + z);

					//setup attachment
					unit->AttachToComponent(self, FAttachmentTransformRules::KeepRelativeTransform);
					unit->SetRelativeLocation(
						FVector(
							2.0f * size.X * (x - count_x),
							2.0f * size.Y * (y - count_y),
							2.0f * size.Z * (z - count_z)
						) + size
					);

					unit->Initialize(boxUnitManager, size, boxUnitManager->m_ConnectType);

					//TODO: move to a function
					//bind pointer
					if (boxUnitManager->m_ConnectType == ConnectType::SixDirection)
					{
						UUnitComponent* neighbor;
						if (x - 1 >= 0)
						{
							neighbor = boxUnitManager->m_Units[(x - 1) * count.Y * count.Z + y * count.Z + z];
							unit->SetNeighbor(BEACON_SIX_DIR_X_BACKWARD, neighbor);
							neighbor->SetNeighbor(BEACON_SIX_DIR_X_FORWARD, unit);
						}
						if (y - 1 >= 0)
						{
							neighbor = boxUnitManager->m_Units[x * count.Y * count.Z + (y - 1) * count.Z + z];
							unit->SetNeighbor(BEACON_SIX_DIR_Y_BACKWARD, neighbor);
							neighbor->SetNeighbor(BEACON_SIX_DIR_Y_FORWARD, unit);
						}
						if (z - 1 >= 0)
						{
							neighbor = boxUnitManager->m_Units[x * count.Y * count.Z + y * count.Z + z - 1];
							unit->SetNeighbor(BEACON_SIX_DIR_Z_BACKWARD, neighbor);
							neighbor->SetNeighbor(BEACON_SIX_DIR_Z_FORWARD, unit);
						}
					}

					boxUnitManager->m_Units.Add(unit);
				}
			}
		}
//TODO: Remove
#ifdef BEACON_DEBUG_1

		TArray<FString> message = {
			"X -1",
			"X +1",
			"Y -1",
			"Y +1",
			"Z -1",
			"Z +1"
		};

		for (auto _unit : boxUnitManager->m_Units)
		{
			BEACON_LOG_FULL(Warning, "Unit is : %s", *(_unit->Debug_Index.ToString()));
			TArray<UUnitComponent*>& tempArr = _unit->GetNeighbors()->neighbors;

			for (int i = 0; i < tempArr.Num(); i++)
			{
				//ignore thermal exchange among burning units
				if (tempArr[i] != nullptr)
				{
					BEACON_LOG(Warning, "Neighbor at %s is : %s", *message[i], *(tempArr[i]->Debug_Index.ToString()));
				}
				else
				{
					BEACON_LOG(Warning, "Neighbor at %s is : NULL", *message[i]);
				}
			}
		}
#endif
	}
};
