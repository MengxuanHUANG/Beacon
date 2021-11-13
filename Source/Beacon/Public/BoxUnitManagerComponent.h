// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitManagerComponent.h"
#include "Components/BoxComponent.h"
#include "BoxUnitManagerComponent.generated.h"

struct UnitCount
{
	UnitCount(uint32 x = 1, uint32 y = 1, uint32 z = 1)
		:X(x), Y(y), Z(z)
	{}

	uint32 X;
	uint32 Y;
	uint32 Z;
};

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

	virtual void OnUnregister() override;

	virtual void UpdateUnits() override;
	virtual UUnitComponent* GetUnit(FVector index) override;
	virtual void TriggerUnit(FVector index) override;
	virtual void SetParameter3(uint32 x, uint32 y, uint32 z) override;
	virtual void SetParticle(UParticleSystem* particle) override;

public:
	UPROPERTY(VisibleAnywhere)
		TArray<UUnitComponent*> m_Units;

	UnitCount m_UnitCount;

	TQueue<UUnitComponent*> m_TriggeredUnits;
	UParticleSystem* m_Particle;

public:
	template<typename InnerUnitType, typename OuterUnitType>
	static void CreateUnit(UBoxUnitManagerComponent* boxUnitManager, USceneComponent* self, const USceneComponent* parent)
	{
		const UBoxComponent* box = Cast<UBoxComponent>(parent);
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
					InnerUnitType* unit = NewObject<InnerUnitType>(self);

					//register component for rendering
					unit->RegisterComponent();
					unit->Initialize(size, boxUnitManager->m_ConnectType);

					//setup attachment
					unit->AttachToComponent(self, FAttachmentTransformRules::KeepRelativeTransform);
					unit->SetRelativeLocation(
						FVector(
							2.0f * size.X * (x - count_x),
							2.0f * size.Y * (y - count_y),
							2.0f * size.Z * (z - count_z)
						) + size
					);

					//TODO: move to a function
					//bind pointer
					if (boxUnitManager->m_ConnectType == ConnectType::SixDirection)
					{
						UUnitComponent* neighbor;
						if (x - 1 >= 0)
						{
							neighbor = boxUnitManager->m_Units[(x - 1) * count.Y * count.Z + y * count.Z + z];
							unit->SetNeighbor(-1, 0, 0, neighbor);
							neighbor->SetNeighbor(1, 0, 0, unit);
						}
						if (y - 1 >= 0)
						{
							neighbor = boxUnitManager->m_Units[x * count.Y * count.Z + (y - 1) * count.Z + z];
							unit->SetNeighbor(0, -1, 0, neighbor);
							neighbor->SetNeighbor(0, 1, 0, unit);
						}
						if (z - 1 >= 0)
						{
							neighbor = boxUnitManager->m_Units[x * count.Y * count.Z + y * count.Z + z - 1];
							unit->SetNeighbor(0, 0, -1, neighbor);
							neighbor->SetNeighbor(0, 0, 1, unit);
						}
					}

					boxUnitManager->m_Units.Add(unit);
				}
			}
		}

#ifdef BEACON_DEBUG
		for (UUnitComponent* unit : boxUnitManager->m_Units)
		{
			if (unit)
			{
				unit->DisplayDebugInfo();
			}
		}
#endif
	}
};
