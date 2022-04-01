// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitComponent.h"
#include "Beacon_PriorityQueue.h"
#include "UnitManagerComponent.h"
#include "Components/CapsuleComponent.h"
#include "CapsuleUnitManagerComponent.generated.h"

/**
 * 
 */
UCLASS()
class BEACON_API UCapsuleUnitManagerComponent : public UUnitManagerComponent
{
	GENERATED_BODY()

public:
	static bool CompareUnit(UUnitComponent* a, UUnitComponent* b);
public:
	UCapsuleUnitManagerComponent();
	~UCapsuleUnitManagerComponent();

	virtual void OnUnregister() override;

	virtual UUnitComponent* GetUnit(FVector index) override;
	virtual void TriggerUnit_Implementation(FVector index, float initValue) override;
	virtual void TriggerUnit_Implementation(UUnitComponent* unit) override;
	virtual void TriggerAllUnits_Implementation(float initValue) override;
	virtual void UnTriggerUnit_Implementation(FVector index, float value) override;
	virtual void UnTriggerAllUnits_Implementation(float value) override;

	virtual void SetReceiveThermalRadiation(bool enable) override;

	virtual void SetParameter(uint32 x) override;
	virtual void SetParameter2(uint32 x, uint32 y) override;
	virtual void SetParameter3(uint32 x, uint32 y, uint32 z) override;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void AddToUpdateList(UUnitComponent* unit) override;

	virtual void SetUnitsMaterial() override;

public:
	UFUNCTION()
		void LateTickComponent(float DeltaTime);

public:
	uint32 m_Count;
	uint32 m_Count_height;
	float last;

	UPROPERTY(VisibleAnywhere)
		TMap<FVector, UUnitComponent*> m_Units;

	Beacon_PriorityQueue<UUnitComponent> m_UpdateList;

	UPROPERTY(VisibleAnywhere)
		const UCapsuleComponent* ParentBox;

public:
	template<typename InnerUnitType, typename OuterUnitType>
	static void CreateUnit(UCapsuleUnitManagerComponent* capsuleUnitManager, USceneComponent* self, const USceneComponent* parent)
	{
		const UCapsuleComponent* capsule = Cast<UCapsuleComponent>(parent);
		float height = 2 * capsule->GetUnscaledCapsuleHalfHeight_WithoutHemisphere();
		float radius = capsule->GetUnscaledCapsuleRadius();
		float size = radius / ((float)(capsuleUnitManager->m_Count) - 0.5f) / FMath::Sqrt(3) /2; // divided by sqrt(2)
		float size_height = height / (float)(capsuleUnitManager->m_Count_height)/2; 
		int count = (int)(capsuleUnitManager->m_Count);
		int count_height = (int)(capsuleUnitManager->m_Count_height);
		BEACON_LOG(Warning, "count is %d", capsuleUnitManager->m_Count);
		BEACON_LOG(Warning, "count is %d", capsuleUnitManager->m_Count_height);
		BEACON_LOG(Warning, "size is %f", size);
		BEACON_LOG(Warning, "size height is %f", size_height);

		BEACON_LOG(Warning, "height is %f", height);
		BEACON_LOG(Warning, "radius is %f", radius);

		int count_t = 0;
		int count_t_new = 0;
		TArray<UUnitComponent*> unitsArray;
		UUnitComponent* unit;
		int tem_count = 0;

		for (int x = -count; x <= count; x++) // Generate lower semi-sphere
		{
			for (int y = -count; y <= count; y++)
			{
				for (int z = 0; z < count; z++)
				{
					if (FMath::Abs(x) + FMath::Abs(y) + FMath::Abs(count-z) <= 2 * (count - 1)+ count)
					{
						if (FMath::Abs(x) + FMath::Abs(y) + FMath::Abs(count-z) == 2 * (count - 1)+ count)
						{
							unit = NewObject<OuterUnitType>(self);
							++count_t;

						}
						else 
						{
							unit = NewObject<InnerUnitType>(self);
							++count_t;
						}
						//register component for rendering
						unit->RegisterComponent();
						unit->Initialize(capsuleUnitManager, FVector(size), capsuleUnitManager->m_ConnectType);
						unit->SetIndex(tem_count);
						++tem_count;

						//setup attachment
						unit->AttachToComponent(self, FAttachmentTransformRules::KeepRelativeTransform);
						unit->SetRelativeLocation(
							FVector(
								2 * x * size,
								2 * y * size,
								-(count_height)*size_height + size + 2 * size * (z-count)
							)
						);

						BEACON_LOG(Display, "Coordinate is %d %d %d", x, y, z);
						capsuleUnitManager->m_Units.Add(FVector(x,y,z),unit);
						unit->Debug_Index = FVector(x, y, z);
						++count_t_new;
					}
				}
			}
		}


		for (int x = -count; x <= count; x++) // generate cylinder
		{
			for (int y = -count; y <= count; y++)
			{
				for (int z = count; z < count+count_height; z++)
				{
					if (FMath::Abs(x) + FMath::Abs(y) <= 2 * (count - 1))
					{
						if (FMath::Abs(x) + FMath::Abs(y) == 2 * (count - 1))
						{
							unit = NewObject<OuterUnitType>(self);
							++count_t;
						}
						else
						{
							unit = NewObject<InnerUnitType>(self);
							++count_t;
						}


						//register component for rendering
						unit->RegisterComponent();
						unit->SetIndex(tem_count);
						++tem_count;
						unit->Initialize(capsuleUnitManager, FVector(size, size, size_height), capsuleUnitManager->m_ConnectType);


						//setup attachment
						unit->AttachToComponent(self, FAttachmentTransformRules::KeepRelativeTransform);
						unit->SetRelativeLocation(
							FVector(
								2 * x * size,
								2 * y * size,
								2 * (z- count) * size_height - (count_height - 1) * size_height
							)
						);

						BEACON_LOG(Display, "Coordinate is %d %d %d", x, y, z);
						capsuleUnitManager->m_Units.Add(FVector(x, y, z), unit);
						unit->Debug_Index = FVector(x, y, z);
						++count_t_new;

					}
				}
			}

		}

		for (int x = -count; x <= count; x++) // Generate upper semi-sphere
		{
			for (int y = -count; y <= count; y++)
			{
				for (int z = count + count_height; z < 2*count + count_height; z++)
				{
					if (FMath::Abs(x) + FMath::Abs(y) + FMath::Abs(z - count - count_height) <= 3 * (count - 1))
					{
						if (FMath::Abs(x) + FMath::Abs(y) + FMath::Abs(z - count - count_height) == 3 * (count - 1))
						{
							unit = NewObject<OuterUnitType>(self);
							++count_t;
						}
						else
						{
							unit = NewObject<InnerUnitType>(self);
							++count_t;
						}


						//register component for rendering
						unit->RegisterComponent();
						unit->SetIndex(tem_count);
						++tem_count;
						unit->Initialize(capsuleUnitManager, FVector(size), capsuleUnitManager->m_ConnectType);

						//setup attachment
						unit->AttachToComponent(self, FAttachmentTransformRules::KeepRelativeTransform);
						unit->SetRelativeLocation(
							FVector(
								2 * x * size,
								2 * y * size,
								count_height*size_height + size + 2 * size * (z- count - count_height)
							)

						);

						BEACON_LOG(Display, "Coordinate is %d %d %d", x, y, z);
						capsuleUnitManager->m_Units.Add(FVector(x, y, z), unit);
						unit->Debug_Index = FVector(x, y, z);
						++count_t_new;

					}
				}
			}
		}

		
		for (int x = -count; x <= count; x++)
		{
			for (int y = -count; y <= count; y++)
			{
				for (int z = 0; z < 2 * count + count_height; z++)
				{
					if (capsuleUnitManager->m_ConnectType == ConnectType::SixDirection)
					{
						UUnitComponent* neighbor;
						if (!capsuleUnitManager->m_Units.Contains(FVector(x,y,z)))
						{
							continue;
						}
						unit = capsuleUnitManager->m_Units[FVector(x, y, z)];
						if (capsuleUnitManager->m_Units.Contains(FVector(x+1, y, z)))
						{
							neighbor = capsuleUnitManager->m_Units[FVector(x+1, y, z)];
							unit->SetNeighbor(BEACON_SIX_DIR_X_FORWARD, neighbor);
							if (neighbor != nullptr)
							{
								neighbor->SetNeighbor(BEACON_SIX_DIR_X_BACKWARD, unit);
							}
						}

						if (capsuleUnitManager->m_Units.Contains(FVector(x, y+1, z)))
						{
							neighbor = capsuleUnitManager->m_Units[FVector(x, y+1, z)];
							unit->SetNeighbor(BEACON_SIX_DIR_Y_FORWARD, neighbor);
							if (neighbor != nullptr)
							{
								neighbor->SetNeighbor(BEACON_SIX_DIR_Y_BACKWARD, unit);
							}
						}

						if (capsuleUnitManager->m_Units.Contains(FVector(x, y, z+1)))
						{
							neighbor = capsuleUnitManager->m_Units[FVector(x, y, z+1)];
							unit->SetNeighbor(BEACON_SIX_DIR_Z_FORWARD, neighbor);
							if (neighbor != nullptr)
							{
								neighbor->SetNeighbor(BEACON_SIX_DIR_Z_BACKWARD, unit);
							}
						}
					}
				}
			}
		}

#ifdef BEACON_DEBUG_1
		TArray<FString> message = {
			"X -1",
			"X +1",
			"Y -1",
			"Y +1",
			"Z -1",
			"Z +1"
		};

		for (auto _unit : capsuleUnitManager->m_Units)
		{
			BEACON_LOG_FULL(Warning, "Unit is : %s", *(_unit.Value->Debug_Index.ToString()));
			TArray<UUnitComponent*>& tempArr = _unit.Value->GetNeighbors()->neighbors;

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

