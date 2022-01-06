// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "BeaconFire.generated.h"


UCLASS( ClassGroup=(Custom), Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class BEACON_API UBeaconFire : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBeaconFire();

	UFUNCTION(BlueprintNativeEvent)
		void StartBurning();

	UFUNCTION(BlueprintNativeEvent)
		void EndBurning();
};
