// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RainSystem.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FDataVector
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		FName Weather;

	UPROPERTY(EditAnywhere)
		FVector2D Weather_Direction;
};

UCLASS()
class BEACON_API URainSystem : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FDataVector m_Weather_Data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Thermal_reduce;

	
};
