// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "BeaconFire.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBeaconBurningEvents_DynamicMulticast, FString, EventName);

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

protected:
	UPROPERTY(BlueprintAssignable, Category = "Beacon Burning Events")
		FBeaconBurningEvents_DynamicMulticast F_BurningEvents;

	UPROPERTY(BlueprintReadWrite)
		float m_FlameStrength;

public:
	UFUNCTION(BlueprintCallable, Category = "Beacon Functions")
		void CallBurningEvent(FString eventName);

	UFUNCTION(BlueprintCallable, Category = "Beacon Functions")
		float GetFlameStrength() const { return  m_FlameStrength; }

	UFUNCTION(BlueprintCallable, Category = "Beacon Functions")
		void SetFlameStrength(float strength) { m_FlameStrength = strength; }
};
