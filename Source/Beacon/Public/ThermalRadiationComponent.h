// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ThermalRadiationComponent.generated.h"

class USphereComponent;
class UUnitComponent;
class UThermalRadiationMaterial;
struct BeaconThermalData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEACON_API UThermalRadiationComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UThermalRadiationComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void DestroyComponent(bool bPromoteChildren) override;

public:
	void Initialize(UThermalRadiationMaterial* material, bool enabled = true);

	inline void BindThermalData(TSharedPtr<BeaconThermalData>& data) { ThermalData = data; }

	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

public:
	UPROPERTY(VisibleAnywhere)
		bool bIsEnabled;

	UPROPERTY(VisibleAnywhere)
		USphereComponent* RadiationSphere;
	
	UPROPERTY(VisibleAnywhere)
		UThermalRadiationMaterial* m_Material;

	TSharedPtr<BeaconThermalData> ThermalData;

	TMap<FString, UUnitComponent*> m_Units;
};
