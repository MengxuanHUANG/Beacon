// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitManagerComponent.h"
#include "Components/SphereComponent.h"
#include "SphereUnitManagerComponent.generated.h"

/**
 * 
 */
UCLASS()
class BEACON_API USphereUnitManagerComponent : public UUnitManagerComponent
{
	GENERATED_BODY()

public:
	USphereUnitManagerComponent();
	~USphereUnitManagerComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnUnregister() override;

	virtual void UpdateUnits() override;
	virtual UUnitComponent* GetUnit(FVector index) override;
	virtual void TriggerUnit(FVector index) override;
	virtual void SetParameter(uint32 x) override;
	virtual void SetBeaconFire(TSubclassOf<UBeaconFire>& beaconFire) override;

public:
	uint32 m_Count;
	
	UPROPERTY(VisibleAnywhere)
		FVector m_UnitCount;

	UPROPERTY(VisibleAnywhere)
		TMap<FVector, UUnitComponent*> m_Units;

	TArray<UUnitComponent*> m_TriggeredUnits;
	
	UPROPERTY(VisibleAnywhere)
		TSubclassOf<UBeaconFire> m_BeaconFire;

public:
	template<typename InnerUnitType, typename OuterUnitType>
	static void CreateUnit(USphereUnitManagerComponent* sphereUnitManager, USceneComponent* self, const USceneComponent* parent)
	{
		const USphereComponent* sphere = Cast<USphereComponent>(parent);
		float radius = sphere->GetUnscaledSphereRadius();
		float size = radius / ((float)(sphereUnitManager->m_Count) - 0.5f) / FMath::Sqrt(3) / 2; // divided by sqrt(2)
		int count = (int)(sphereUnitManager->m_Count);
		//int count = radius / m_UnitExtent.X;

		TArray<UUnitComponent*> unitsArray;
		UUnitComponent* unit;
		for (int x = -count; x <= count; x++)
		{
			for (int y = -count; y <= count; y++)
			{
				for (int z = -count; z <= count; z++)
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
						unit->Initialize(FVector(size), sphereUnitManager->m_ConnectType);

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
						if (sphereUnitManager->m_ConnectType == ConnectType::SixDirection)
						{
							UUnitComponent* neighbor;
							if (x - 1 >= 0)
							{
								neighbor = unitsArray[
									(count + x - 1) * sphereUnitManager->m_Count * sphereUnitManager->m_Count 
										+ (count + y) * sphereUnitManager->m_Count
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
									(count + x) * sphereUnitManager->m_Count * sphereUnitManager->m_Count 
										+ (count + y - 1) * sphereUnitManager->m_Count
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
									(count + x) * sphereUnitManager->m_Count * sphereUnitManager->m_Count 
										+ (count + y) * sphereUnitManager->m_Count 
										+ count + z - 1];
								unit->SetNeighbor(0, 0, -1, neighbor);
								if (neighbor != nullptr)
								{
									neighbor->SetNeighbor(0, 0, 1, unit);
								}
							}
						}

						unitsArray.Add(unit);
						sphereUnitManager->m_Units.Add(FVector(x, y, z), unit);
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
