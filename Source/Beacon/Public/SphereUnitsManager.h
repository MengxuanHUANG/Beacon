// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BeaconCore.h"

#include "Unit.h"
#include "UnitsManager.h"

#include "Components/SphereComponent.h"
/**
 * 
 */
template<typename InnerUnitType, typename OuterUnitType>
class BEACON_API SphereUnitsManager: public UnitsManager
{
public:
	SphereUnitsManager(ConnectType type, uint32 count)
		:UnitsManager(type), m_Count(count)
	{}
	~SphereUnitsManager() 
	{
	}
	virtual void CreateUnits(USceneComponent* self, const USceneComponent* parent) override
	{
		const USphereComponent* sphere = Cast<USphereComponent>(parent);
		float radius = sphere->GetUnscaledSphereRadius();
		float size = radius / ((float)m_Count - 0.5f) / FMath::Sqrt(3) / 2; // divided by sqrt(2)
		int count = ((int)m_Count);
		//int count = radius / m_UnitExtent.X;

		TArray<Unit*> unitsArray;

		for (int x = -count; x <= count; x++)
		{
			for (int y = -count; y <= count; y++)
			{
				for (int z = -count; z <= count; z++)
				{
					if (FMath::Abs(x) + FMath::Abs(y) + FMath::Abs(z) <= 3 * (count - 1))
					{
						InnerUnitType* unit = NewObject<InnerUnitType>(self);

						//register component for rendering
						unit->RegisterComponent();
						unit->Initialize(FVector(size), m_ConnectType);

						//setup attachment
						unit->AttachToComponent(self, FAttachmentTransformRules::KeepRelativeTransform);
						unit->SetRelativeLocation(
							FVector(
								2 * x * size,
								2 * y * size,
								2 * z * size
							)
						);

						//bind pointer
						if (m_ConnectType == ConnectType::SixDirection)
						{
							Unit* neighbor;
							if (x - 1 >= 0)
							{
								neighbor = unitsArray[(count + x - 1) * m_Count * m_Count + (count + y) * m_Count + (count + z)];
								unit->SetNeighbor(-1, 0, 0, neighbor);
								if (neighbor != nullptr)
								{
									neighbor->SetNeighbor(1, 0, 0, unit);
								}
							}
							if (y - 1 >= 0)
							{
								neighbor = unitsArray[(count + x) * m_Count * m_Count + (count + y - 1) * m_Count + (count + z)];
								unit->SetNeighbor(0, -1, 0, neighbor);
								if (neighbor != nullptr)
								{
									neighbor->SetNeighbor(0, 1, 0, unit);
								}
							}
							if (z - 1 >= 0)
							{
								neighbor = unitsArray[(count + x) * m_Count * m_Count + (count + y) * m_Count + count + z - 1];
								unit->SetNeighbor(0, 0, -1, neighbor);
								if (neighbor != nullptr)
								{
									neighbor->SetNeighbor(0, 0, 1, unit);
								}
							}
						}

						unitsArray.Add(unit);
						m_Units.Add(FVector(x, y, z), unit);
					}
					else
					{
						unitsArray.Add(nullptr);
					}
				}
			}
		}

		unitsArray.Empty();
	}
	virtual Unit* GetUnit(FVector index) override
	{
		return nullptr;
	}
	virtual void UpdateUnits() override
	{

	}

public:
	uint32 m_Count;
	TMap<FVector, Unit*> m_Units;
};
