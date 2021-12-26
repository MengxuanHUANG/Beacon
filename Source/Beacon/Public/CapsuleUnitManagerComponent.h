// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	UCapsuleUnitManagerComponent();
	~UCapsuleUnitManagerComponent();

	virtual void OnUnregister() override;

	virtual void UpdateUnits() override;
	virtual UUnitComponent* GetUnit(FVector index) override;
	virtual void TriggerUnit(FVector index) override;
	virtual void SetParameter(uint32 x) override;
	virtual void SetParameter2(uint32 x, uint32 y) override;
	virtual void SetParameter3(uint32 x, uint32 y, uint32 z) override;

	virtual void SetParticle(UParticleSystem* particle) override;

public:
	uint32 m_Count;

	UPROPERTY(VisibleAnywhere)
		TMap<FVector, UUnitComponent*> m_Units;

public:
	template<typename InnerUnitType, typename OuterUnitType>
	static void CreateUnit(UCapsuleUnitManagerComponent* capsuleUnitManager, USceneComponent* self, const USceneComponent* parent)
	{
		const UCapsuleComponent* capsule = Cast<UCapsuleComponent>(parent);
		float height = 2 * capsule->GetUnscaledCapsuleHalfHeight_WithoutHemisphere();
		float radius = capsule->GetUnscaledCapsuleRadius();
		float size = radius / ((float)(capsuleUnitManager->m_Count) - 0.5f) / FMath::Sqrt(3) / 2; // divided by sqrt(2)
		int count = (int)(capsuleUnitManager->m_Count);

		BEACON_LOG(Warning, "count is %d", capsuleUnitManager->m_Count);
		BEACON_LOG(Warning, "size is %f", size);

		BEACON_LOG(Warning, "height is %f", height);
		BEACON_LOG(Warning, "radius is %f", radius);


		TArray<UUnitComponent*> unitsArray;
		for (int x = -count; x <= count; x++) // Generate two semi-sphere
		{
			for (int y = -count; y <= count; y++)
			{
				for (int z = -count; z <= 0; z++)
				{
					if (FMath::Abs(x) + FMath::Abs(y) + FMath::Abs(z) <= 3 * (count - 1))
					{
						InnerUnitType* unit = NewObject<InnerUnitType>(self);

						//register component for rendering
						unit->RegisterComponent();
						unit->Initialize(FVector(size), capsuleUnitManager->m_ConnectType);

						//setup attachment
						unit->AttachToComponent(self, FAttachmentTransformRules::KeepRelativeTransform);
						unit->SetRelativeLocation(
							FVector(
								2 * x * size,
								2 * y * size,
								2 * z * size - height / 2
							)

						);

						/*
						BEACON_LOG(Warning, "x is %f", 2 * x * size);
						BEACON_LOG(Warning, "y is %f", 2 * y * size);
						BEACON_LOG(Warning, "z is %f", 2 * z * size);
						*/


						//bind pointer
						if (capsuleUnitManager->m_ConnectType == ConnectType::SixDirection)
						{
							UUnitComponent* neighbor;
							if (x - 1 >= 0)
							{
								neighbor = unitsArray[
									(count + x - 1) * capsuleUnitManager->m_Count * capsuleUnitManager->m_Count
										+ (count + y) * capsuleUnitManager->m_Count
										+ (count + z)];
								unit->SetNeighbor(-1, 0, 0, neighbor);
								if (neighbor != nullptr)
								{
									neighbor->SetNeighbor(1, 0, 0, unit);
								}
							}
							if (y - 1 >= 0)
							{
								neighbor = unitsArray[
									(count + x) * capsuleUnitManager->m_Count * capsuleUnitManager->m_Count
										+ (count + y - 1) * capsuleUnitManager->m_Count
										+ (count + z)];
								unit->SetNeighbor(0, -1, 0, neighbor);
								if (neighbor != nullptr)
								{
									neighbor->SetNeighbor(0, 1, 0, unit);
								}
							}
							if (z - 1 >= 0)
							{
								neighbor = unitsArray[
									(count + x) * capsuleUnitManager->m_Count * capsuleUnitManager->m_Count
										+ (count + y) * capsuleUnitManager->m_Count
										+ count + z - 1];
								unit->SetNeighbor(0, 0, -1, neighbor);
								if (neighbor != nullptr)
								{
									neighbor->SetNeighbor(0, 0, 1, unit);
								}
							}
						}

						unitsArray.Add(unit);
						capsuleUnitManager->m_Units.Add(FVector(x, y, z), unit);
					}
					else
					{
						unitsArray.Add(nullptr);
					}
				}
				for (int z_upper = count; z_upper >= 0; z_upper--)
				{
					if (FMath::Abs(x) + FMath::Abs(y) + FMath::Abs(z_upper) <= 3 * (count - 1))
					{
						InnerUnitType* unit = NewObject<InnerUnitType>(self);

						//register component for rendering
						unit->RegisterComponent();
						unit->Initialize(FVector(size), capsuleUnitManager->m_ConnectType);

						//setup attachment
						unit->AttachToComponent(self, FAttachmentTransformRules::KeepRelativeTransform);
						unit->SetRelativeLocation(
							FVector(
								2 * x * size,
								2 * y * size,
								2 * z_upper * size + height / 2
							)

						);

						/*
						BEACON_LOG(Warning, "x is %f", 2 * x * size);
						BEACON_LOG(Warning, "y is %f", 2 * y * size);
						BEACON_LOG(Warning, "z is %f", 2 * z * size);
						*/


						//bind pointer
						if (capsuleUnitManager->m_ConnectType == ConnectType::SixDirection)
						{
							UUnitComponent* neighbor;
							if (x - 1 >= 0)
							{
								neighbor = unitsArray[
									(count + x - 1) * capsuleUnitManager->m_Count * capsuleUnitManager->m_Count
										+ (count + y) * capsuleUnitManager->m_Count
										+ (count + z_upper)];
								unit->SetNeighbor(-1, 0, 0, neighbor);
								if (neighbor != nullptr)
								{
									neighbor->SetNeighbor(1, 0, 0, unit);
								}
							}
							if (y - 1 >= 0)
							{
								neighbor = unitsArray[
									(count + x) * capsuleUnitManager->m_Count * capsuleUnitManager->m_Count
										+ (count + y - 1) * capsuleUnitManager->m_Count
										+ (count + z_upper)];
								unit->SetNeighbor(0, -1, 0, neighbor);
								if (neighbor != nullptr)
								{
									neighbor->SetNeighbor(0, 1, 0, unit);
								}
							}
							if (z_upper - 1 >= 0)
							{
								neighbor = unitsArray[
									(count + x) * capsuleUnitManager->m_Count * capsuleUnitManager->m_Count
										+ (count + y) * capsuleUnitManager->m_Count
										+ count + z_upper - 1];
								unit->SetNeighbor(0, 0, -1, neighbor);
								if (neighbor != nullptr)
								{
									neighbor->SetNeighbor(0, 0, 1, unit);
								}
							}
						}

						unitsArray.Add(unit);
						capsuleUnitManager->m_Units.Add(FVector(x, y, z_upper), unit);
					}
					else
					{
						unitsArray.Add(nullptr);
					}
				}

			}
		}

		int box_size = 5;
		for (int x = -radius / 2; x <= radius / 2; x = x + box_size)
		{
			for (int y = -radius / 2; y <= radius / 2; y = y + box_size)
			{
				if (sqrt(x * x + y * y) > radius / 2)
				{
					continue;
				}
				for (int z = -height / 4; z <= height / 4; z = z + box_size)
				{
					BEACON_LOG(Warning, "did");
					if (sqrt(x * x + y * y) <= radius / 2)
					{
						InnerUnitType* unit = NewObject<InnerUnitType>(self);

						//register component for rendering
						unit->RegisterComponent();
						unit->Initialize(FVector(box_size), capsuleUnitManager->m_ConnectType);

						//setup attachment
						unit->AttachToComponent(self, FAttachmentTransformRules::KeepRelativeTransform);
						unit->SetRelativeLocation(
							FVector(
								2 * x,
								2 * y,
								2 * z
							)

						);

						/*
						BEACON_LOG(Warning, "x is %f", 2 * x * size);
						BEACON_LOG(Warning, "y is %f", 2 * y * size);
						BEACON_LOG(Warning, "z is %f", 2 * z * size);
						*/

						/*
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
				for (int z_upper = count; z_upper >= 0; z_upper--)
				{
					if (FMath::Abs(x) + FMath::Abs(y) + FMath::Abs(z_upper) <= 3 * (count - 1))
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
								2 * z_upper * size + height / 2
							)

						);


						BEACON_LOG(Warning, "x is %f", 2 * x * size);
						BEACON_LOG(Warning, "y is %f", 2 * y * size);
						BEACON_LOG(Warning, "z is %f", 2 * z * size);



						//bind pointer
						if (m_ConnectType == ConnectType::SixDirection)
						{
							Unit* neighbor;
							if (x - 1 >= 0)
							{
								neighbor = unitsArray[(count + x - 1) * m_Count * m_Count + (count + y) * m_Count + (count + z_upper)];
								unit->SetNeighbor(-1, 0, 0, neighbor);
								if (neighbor != nullptr)
								{
									neighbor->SetNeighbor(1, 0, 0, unit);
								}
							}
							if (y - 1 >= 0)
							{
								neighbor = unitsArray[(count + x) * m_Count * m_Count + (count + y - 1) * m_Count + (count + z_upper)];
								unit->SetNeighbor(0, -1, 0, neighbor);
								if (neighbor != nullptr)
								{
									neighbor->SetNeighbor(0, 1, 0, unit);
								}
							}
							if (z_upper - 1 >= 0)
							{
								neighbor = unitsArray[(count + x) * m_Count * m_Count + (count + y) * m_Count + count + z_upper - 1];
								unit->SetNeighbor(0, 0, -1, neighbor);
								if (neighbor != nullptr)
								{
									neighbor->SetNeighbor(0, 0, 1, unit);
								}
							}
						}

						unitsArray.Add(unit);
						m_Units.Add(FVector(x, y, z_upper), unit);
						*/
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
};
