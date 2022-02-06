// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BeaconFire.h"
#include "BeaconMaterial.h"
#include "FractureMaterial.generated.h"

USTRUCT(BlueprintType)
struct FFragmentLevel
{
	GENERATED_BODY()

public:
	/** Proportion of burning debris */
	UPROPERTY(EditAnywhere)
		float Debris_BurningRate;

	/** BeaconMaterial for this level */
	UPROPERTY(EditAnywhere)
		UBeaconMaterial* BeaconMaterial;

	UPROPERTY(EditAnywhere)
		FVector Debris_Size;

	/** minimum time of debris can burn */
	UPROPERTY(EditAnywhere)
		TSubclassOf<UBeaconFire> T_BeaconFire;
};

/**
 * 
 */
UCLASS()
class BEACON_API UFractureMaterial : public UDataAsset
{
	GENERATED_BODY()

public:
	//Burning time of different levels, should not exceed MaxDebrisLevel
	UPROPERTY(EditAnywhere)
		TArray<FFragmentLevel> FragmentLevels;
};
