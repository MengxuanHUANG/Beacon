// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ThermalRadiationMaterial.generated.h"

/**
 * 
 */
UCLASS()
class BEACON_API UThermalRadiationMaterial : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Whether to enable attenuation for thermal transmition */
	UPROPERTY(EditAnywhere)
		float MAX_Radius;

	/** Default thermal transmition */
	UPROPERTY(EditAnywhere)
		float Thermal_transmit;

	/** Whether to enable attenuation for thermal transmit */
	UPROPERTY(EditAnywhere, Category = "Attenuation")
		bool EnableAttenuation;

	/** Distance that start attenuation*/
	UPROPERTY(EditAnywhere, Category = "Attenuation")
		float Attenuation_Start_Radius;

	/** Define relationship of distance and thermal transmit 
	* X-axis: distance, should between Attenuation_Start_Radius and MAX_Radius
	* Y-axis: Coefficient, will multiply to Thermal_transmit
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attenuation")
		UCurveFloat* Distance_Curve;
};
