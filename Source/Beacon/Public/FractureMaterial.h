// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BeaconFire.h"
#include "FractureMaterial.generated.h"

USTRUCT(BlueprintType)
struct FBurningTime
{
	GENERATED_BODY()

public:
	//maximum time of debris can burn
	UPROPERTY(EditAnywhere)
		float MaxBurningTime;

	//minimum time of debris can burn
	UPROPERTY(EditAnywhere)
		float MinBurningTime;

	UPROPERTY(EditAnywhere)
		FVector DebrisSize;

	//minimum time of debris can burn
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
	//Proportion of burning debris
	UPROPERTY(EditAnywhere)
		float DebrisBurningRate;

	UPROPERTY(EditAnywhere)
		bool HasMaxBurningTime;
	
	//Maximum levels of debris
	UPROPERTY(EditAnywhere)
		uint8 MaxDebrisLevel;

	//Burning time of different levels, should not exceed MaxDebrisLevel
	UPROPERTY(EditAnywhere)
		TArray<FBurningTime> BurningTime;
};
