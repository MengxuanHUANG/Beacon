// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitManagerComponent.h"
#include "Components/SphereComponent.h"

#include "BeaconLog.h"
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

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnUnregister() override;

	virtual UUnitComponent* GetUnit(FVector index) override;
	virtual void SetParameter(uint32 x) override;

	virtual void TriggerUnit_Implementation(UUnitComponent* unit) override;
	virtual void TriggerUnit_Implementation(FVector index, float initValue) override;
	virtual void TriggerAllUnits_Implementation(float initValue) override;
	virtual void UnTriggerUnit_Implementation(FVector index, float value) override;
	virtual void UnTriggerAllUnits_Implementation(float value) override;

	virtual void SetUnitsMaterial() override;

	virtual void AddToUpdateList(UUnitComponent* unit) override;

public:
	UFUNCTION()
		void LateTickComponent(float DeltaTime);

public:
	uint32 m_Count;
	
	UPROPERTY(VisibleAnywhere)
		FVector m_UnitCount;

	UPROPERTY(VisibleAnywhere)
		TMap<FVector, UUnitComponent*> m_Units;

	Beacon_PriorityQueue<UUnitComponent> m_UpdateList;
	
	UPROPERTY(VisibleAnywhere)
		TSubclassOf<UBeaconFire> m_BeaconFire;

private:
	static bool CompareUnits(UUnitComponent* a, UUnitComponent* b);

public:
	template<typename InnerUnitType, typename OuterUnitType>
	static void CreateUnit(USphereUnitManagerComponent* sphereUnitManager, USceneComponent* self, const USceneComponent* parent)
	{
		const USphereComponent* sphere = Cast<USphereComponent>(parent);
		float radius = sphere->GetUnscaledSphereRadius();
		float size = radius / ((float)(sphereUnitManager->m_Count) - 0.5f) / FMath::Sqrt(3) / 2; // divided by sqrt(2)
		int count = (int)(sphereUnitManager->m_Count);
		int loop_size = (count<<1) | 1;
		//int count = radius / m_UnitExtent.X;

		TArray<UUnitComponent*> unitsArray;
		UUnitComponent* unit;

		int index = 0;

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
						else if(FMath::Abs(x) == count || FMath::Abs(y) == count || FMath::Abs(z) == count)
						{
							unit = NewObject<OuterUnitType>(self);
						}
						else
						{
							unit = NewObject<InnerUnitType>(self);
						}
						

						//register component for rendering
						unit->RegisterComponent();
						unit->Initialize(sphereUnitManager, FVector(size), sphereUnitManager->m_ConnectType);

						unit->SetIndex(index);
						++index;
						
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
							if (x > -count)
							{
								neighbor = unitsArray[
									(count + x - 1) * loop_size * loop_size
										+ (count + y) * loop_size
										+ (count + z)];
								unit->SetNeighbor(BEACON_SIX_DIR_X_BACKWARD, neighbor);
								if (neighbor != nullptr)
								{
									neighbor->SetNeighbor(BEACON_SIX_DIR_X_FORWARD, unit);
								}
							}
							if (y > -count)
							{
								neighbor = unitsArray[
									(count + x) * loop_size * loop_size
										+ (count + y - 1) * loop_size
										+ (count + z)];
								unit->SetNeighbor(BEACON_SIX_DIR_Y_BACKWARD, neighbor);
								if (neighbor != nullptr)
								{
									neighbor->SetNeighbor(BEACON_SIX_DIR_Y_FORWARD, unit);
								}
							}
							if (z > -count)
							{
								neighbor = unitsArray[
									(count + x) * loop_size * loop_size
										+ (count + y) * loop_size
										+ count + z - 1];
								unit->SetNeighbor(BEACON_SIX_DIR_Z_BACKWARD, neighbor);
								if (neighbor != nullptr)
								{
									neighbor->SetNeighbor(BEACON_SIX_DIR_Z_FORWARD, unit);
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
