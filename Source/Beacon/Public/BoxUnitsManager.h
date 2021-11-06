// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BeaconCore.h"

#include "Unit.h"
#include "UnitsManager.h"

#include "Components/BoxComponent.h"

struct UnitsCount
{
	UnitsCount(uint32 x, uint32 y, uint32 z)
		:X(x), Y(y), Z(z)
	{}

	uint32 X;
	uint32 Y;
	uint32 Z;
};

template<typename InnerUnitType, typename OuterUnitType>
class BEACON_API BoxUnitsManager : public UnitsManager
{
public:
	BoxUnitsManager(ConnectType type, uint32 x, uint32 y, uint32 z)
		:UnitsManager(type), m_UnitCount(x, y, z)
	{}
	~BoxUnitsManager() 
	{

	}

	virtual void Destroy(bool bPromoteChildren) override
	{
		for (Unit* unit : m_Units)
		{
			unit->OnDestroy(bPromoteChildren);
		}
		m_Units.Empty();
	}

	virtual void CreateUnits(USceneComponent* self, const USceneComponent* parent) override
	{
		const UBoxComponent* box = Cast<UBoxComponent>(parent);
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
		m_Units.Reserve(m_Units.Num() + m_UnitCount.X * m_UnitCount.Y * m_UnitCount.Z);

		for (int x = 0; x < (int)m_UnitCount.X; x++)
		{
			for (int y = 0; y < (int)m_UnitCount.Y; y++)
			{
				for (int z = 0; z < (int)m_UnitCount.Z; z++)
				{
					InnerUnitType* unit = NewObject<InnerUnitType>(self);

					//register component for rendering
					unit->RegisterComponent();
					unit->Initialize(size, m_ConnectType);

					//setup attachment
					unit->AttachToComponent(self, FAttachmentTransformRules::KeepRelativeTransform);
					unit->SetRelativeLocation(
						FVector(
							2.0f * size.X * (x - count_x),
							2.0f * size.Y * (y - count_y),
							2.0f * size.Z * (z - count_z)
						) + size
					);

					//bind pointer
					if (m_ConnectType == ConnectType::SixDirection)
					{
						Unit* neighbor;
						if (x - 1 >= 0)
						{
							neighbor = m_Units[(x - 1) * count_y * count_z + y * count_z + z];
							unit->SetNeighbor(-1, 0, 0, neighbor);
							neighbor->SetNeighbor(1, 0, 0, unit);
						}
						if (y - 1 >= 0)
						{
							neighbor = m_Units[x * count_y * count_z + (y - 1) * count_z + z];
							unit->SetNeighbor(0, -1, 0, neighbor);
							neighbor->SetNeighbor(0, 1, 0, unit);
						}
						if (z - 1 >= 0)
						{
							neighbor = m_Units[x * count_y * count_z + y * count_z + z - 1];
							unit->SetNeighbor(0, 0, -1, neighbor);
							neighbor->SetNeighbor(0, 0, 1, unit);
						}
					}

					m_Units.Add(unit);
				}
			}
		}

#ifdef BEACON_DEBUG
		for (int x = 0; x < (int)m_UnitCount.X; x++)
		{
			for (int y = 0; y < (int)m_UnitCount.Y; y++)
			{
				for (int z = 0; z < (int)m_UnitCount.Z; z++)
				{
					BEACON_LOG(Display, "Index %f", x * count_y * count_z + y * count_z + z);
				}
			}
		}
		/*for (Unit* unit : m_Units)
		{
			if (unit)
			{
				unit->DisplayDebugInfo();
			}
		}*/
#endif
	}
	virtual void UpdateUnits() override
	{
		if (m_ConnectType != ConnectType::None)
		{

		}
	}
	virtual Unit* GetUnit(FVector index) override
	{
		//TODO: implement the search function
		return nullptr;
	}
private:
	TArray<Unit*> m_Units;
	UnitsCount m_UnitCount;
};
