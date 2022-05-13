// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeatherSystem.generated.h"

class UBoxComponent;
class URainSystem;

UCLASS()
class BEACON_API AWeatherSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeatherSystem();
	UPROPERTY(VisibleAnyWhere)
		UBoxComponent* RainColliderBox;

	UPROPERTY(VisibleAnyWhere)
		UBoxComponent* RainColliderPlane;

	UPROPERTY(VisibleAnyWhere)
		UParticleSystemComponent* RainParticle;

	UPROPERTY(EditAnywhere)
		URainSystem* T_WeatherSystem;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> m_IgnoredActors;
	

public:
	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);
			

	UFUNCTION()
		void OnEndOverlap(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	TSet<UUnitComponent*> FlammableUnitSet;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FVector m_Rain_Direction;
	float speed_coefficient;

public:
	UFUNCTION()
		void CastRay(const AActor* actor, FHitResult& result, FVector direction, float length, FVector start_direction);

protected:
	bool m_WeatherActive;

public:
	UFUNCTION(BlueprintCallable)
		void WeatherActive(bool ifactive);
};
