// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitManagerComponent.h"
#include "FlammableUnitComponent.h"
#include "DebrisUnitManagerComponent.generated.h"

class UFractureComponent;
class UFractureMaterial;
class UBeaconMaterial;
class UnitUpdater;

/**
 * 
 */
UCLASS()
class BEACON_API UDebrisUnitManagerComponent : public USceneComponent
{
	GENERATED_BODY()
public:
	UDebrisUnitManagerComponent();
	~UDebrisUnitManagerComponent();
public:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnUnregister() override;

public:
	void Initialize(UFractureComponent*  fractureComponent, bool isFractured = false);
	void SetFractureMaterial(UFractureMaterial* fractureMaterial);

private:
	UFUNCTION()
		void OnFractured(bool needUpdate);

	UFUNCTION()
		void LateTickComponent(float DeltaTime);
public:
	UPROPERTY(VisibleAnywhere)
		UFractureComponent* m_FractureComponent;

	UPROPERTY(VisibleAnywhere)
		UFractureMaterial* m_FractureMaterial;

	UPROPERTY(VisibleAnywhere)
		TArray<UBeaconMaterial*> m_UnitMaterials;

	UPROPERTY(VisibleAnywhere)
		TMap<int32, UFlammableUnitComponent*> m_FlammableUnits;

protected:
	UFUNCTION()
		void UpdateFlammableUnits(float deltaTime);

private:
	bool bIsFractured;
};
