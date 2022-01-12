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

	virtual void SetBeaconFire(TSubclassOf<UBeaconFire>& beaconFire) override;

public:
	uint32 m_Count;
	uint32 m_Count_height;

	UPROPERTY(VisibleAnywhere)
		TMap<FVector, UUnitComponent*> m_Units;

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


		TArray<UUnitComponent*> unitsArray;
		UUnitComponent* unit;
		for (int x = -count; x <= count; x++) // Generate two semi-sphere
		{
			for (int y = -count; y <= count; y++)
			{
				for (int z = -count; z <= 0; z++)
				{
					if (FMath::Abs(x) + FMath::Abs(y) + FMath::Abs(z) <= 3 * (count - 1))
					{
						if (FMath::Abs(x) + FMath::Abs(y) + FMath::Abs(z) == 3 * (count - 1))
						{
							unit = NewObject<OuterUnitType>(self);
						}
						else 
						{
							unit = NewObject<InnerUnitType>(self);
						}
						//register component for rendering
						unit->RegisterComponent();
						unit->Initialize(FVector(size), capsuleUnitManager->m_ConnectType);

						//setup attachment
						unit->AttachToComponent(self, FAttachmentTransformRules::KeepRelativeTransform);
						unit->SetRelativeLocation(
							FVector(
								2 * x * size,
								2 * y * size,
								2 * (z - 0.5) * size - height / 2
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
				for (int z = count; z >= 0; z--)
				{
					if (FMath::Abs(x) + FMath::Abs(y) + FMath::Abs(z) <= 3 * (count - 1))
					{
						if (FMath::Abs(x) + FMath::Abs(y) + FMath::Abs(z) == 3 * (count - 1))
						{
							unit = NewObject<OuterUnitType>(self);
						}
						else
						{
							unit = NewObject<InnerUnitType>(self);
						}
						

						//register component for rendering
						unit->RegisterComponent();
						unit->Initialize(FVector(size), capsuleUnitManager->m_ConnectType);

						//setup attachment
						unit->AttachToComponent(self, FAttachmentTransformRules::KeepRelativeTransform);
						unit->SetRelativeLocation(
							FVector(
								2 * x * size,
								2 * y * size,
								2 * (z + 0.5) * size + height / 2
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

			}
		}



		for (int x = -count; x <= count; x++) // generate cylinder
		{
			for (int y = -count; y <= count; y++)
			{
				for (int z = 0; z < count_height; z++)
				{
					if (FMath::Abs(x) + FMath::Abs(y) <= 2 * (count-1) )
					{
						if (FMath::Abs(x) + FMath::Abs(y) == 2 * (count - 1))
						{
							unit = NewObject<OuterUnitType>(self);
						}
						else
						{
							unit = NewObject<InnerUnitType>(self);
						}
						

						//register component for rendering
						unit->RegisterComponent();
						unit->Initialize(FVector(size, size, size_height), capsuleUnitManager->m_ConnectType);

						//setup attachment
						unit->AttachToComponent(self, FAttachmentTransformRules::KeepRelativeTransform);
						unit->SetRelativeLocation(
							FVector(
								2 * x * size,
								2 * y * size,
								2 * (z-(count_height-1)/2) * size_height
							)

						);


						//bind pointer
						/*if (capsuleUnitManager->m_ConnectType == ConnectType::SixDirection)
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
						}*/

						unitsArray.Add(unit);
						capsuleUnitManager->m_Units.Add(FVector(x, y, z), unit);
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