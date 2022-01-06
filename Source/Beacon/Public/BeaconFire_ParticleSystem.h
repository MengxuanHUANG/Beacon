// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BeaconFire.h"
#include "BeaconFire_ParticleSystem.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class BEACON_API UBeaconFire_ParticleSystem : public UBeaconFire
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UBeaconFire_ParticleSystem();

	void Initialize();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystemComponent* m_Particle;
};
