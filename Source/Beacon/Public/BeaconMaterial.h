// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BeaconMaterial.generated.h"

/**
 * 
 */

class UCurveFloat;

USTRUCT(BlueprintType)
struct FPair
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
		float Value;
	
	UPROPERTY(EditAnywhere)
		FString FunctionName;
};

UENUM(BlueprintType)
enum class ObjectTemplate : uint8
{
	None = 0,
	AlwaysBurn,
	Break,
	Melt,
};

UCLASS()
class BEACON_API UBeaconMaterial : public UDataAsset
{
	GENERATED_BODY()
public:
	UBeaconMaterial();

public:
	/** Effect template */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ObjectTemplate Template = ObjectTemplate::None;
	
	/** Default thermal */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DefaultThermal = 0;

	/** Temperature that start to burn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Flash_Point = 100; 
	
	/** Coefficient of thermal exchange (between 0 and 1)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Thermal_Exchange_Coefficient = 0.5;

	/** Define relationship of thermal and temperature */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCurveFloat* Temperature_Curve; 

	/** Whether the object can always burn*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Has_Max_BurningTime = false;
	
	/** Minimum time (in second) that object can burn*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Min_BurningTime = 0;

	/** Maximum time (in second) that object can burn*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Max_BurningTime = 0;

	/** Thermal Generated Per Second*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float GenThermalPerSecond = 1;
	
	/** Thermal Loss Per Second*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LoseThermalPerSecond = 1;

	/** Maximum Thermal*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MAX_Thermal = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FPair> BurningEvents;

public:
	UFUNCTION()
		ObjectTemplate GetObjectTemplate() const;

	UFUNCTION()
		FORCEINLINE float GetTemperature(uint32 thermal) const;
};
