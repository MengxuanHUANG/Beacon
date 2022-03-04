// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildableComponent.h"
#include "Neighbor.h"
#include "UnitManagerComponent.h"
#include "BeaconFire.h"

#include "FlammableComponent.generated.h"

class UParticleSystemComponent;
class UFlammableUnitComponent;
class UBeaconMaterial;
class UThermalRadiationMaterial;
class UThermalRadiationComponent;

USTRUCT(BlueprintType)
struct FUnitCount
{
	GENERATED_BODY()
public:
	FUnitCount() {}
	FUnitCount(uint32 x, uint32 y, uint32 z)
		:X(x), Y(y), Z(z)
	{
	}
	UPROPERTY(EditAnywhere)
		uint32 X;
	UPROPERTY(EditAnywhere)
		uint32 Y;
	UPROPERTY(EditAnywhere)
		uint32 Z;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEACON_API UFlammableComponent : public UBuildableComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFlammableComponent();
	~UFlammableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void DestroyComponent(bool bPromoteChildren) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool InitializeWithFlame;

	UPROPERTY(EditAnywhere)
		bool EnableThermalProxy;

	UPROPERTY(EditAnywhere, Category = "FlammableComponent | Beacon Fire")
		TSubclassOf<UBeaconFire> T_BeaconFire;

	UPROPERTY(EditAnywhere, Category = "FlammableComponent | Flammable Material")
		UBeaconMaterial* T_Material;

	UPROPERTY(EditAnywhere, Category = "FlammableComponent | Thermal Radiation")
		bool bEnableReceiveThermalRadiation;

	UPROPERTY(EditAnywhere, Category = "FlammableComponent | Thermal Radiation")
		bool bEnableSendThermalRadiation;

	UPROPERTY(EditAnywhere, Category = "FlammableComponent | Thermal Radiation")
		UThermalRadiationMaterial* T_RadiationMaterial;

	UPROPERTY(VisibleAnywhere, Category = "FlammableComponent | Thermal Radiation")
		UThermalRadiationComponent* m_ThermalRadiationComponent;

	UPROPERTY(EditAnywhere)
		uint32 m_Count = 1;

	UPROPERTY(EditAnywhere)
		FUnitCount m_UnitCount = {2, 2, 2};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ConnectType m_ConnectType = ConnectType::SixDirection;

	UPROPERTY(VisibleAnywhere)
		UUnitManagerComponent* m_UnitManager;

protected:
	//Begin declaring BuildableComponent functions
	virtual bool Build_Implement() override;
	virtual	void Clear_Implement() override;
	//End declaring BuildableComponent functions

	class UFractureComponent* GetFractureComponent() const;
	void ConfigObjectTemplate(ObjectTemplate objTemplate);
	
	UFUNCTION()
		void OnFractured(bool needUpdate);
public:
	UFUNCTION(BlueprintCallable)
		void BurnAll();

	UFUNCTION()
		inline const TSubclassOf<UBeaconFire>& GetBeaconFire() const { return T_BeaconFire; }
};
