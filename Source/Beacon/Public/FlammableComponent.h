// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlammableComponent.generated.h"

class UParticleSystemComponent;
class UStaticMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEACON_API UFlammableComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFlammableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	virtual void DestroyComponent(bool bPromoteChildren) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_InitializeWithFlame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool b_IsBurning;

	UPROPERTY(EditAnywhere)
		UParticleSystem* T_FireParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UParticleSystemComponent* m_ParticleSystem;

	UFUNCTION(BlueprintCallable)
		void Ignited(UParticleSystem* particle);

	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

public:
	UFUNCTION()
		inline bool IsBurning() const { return b_IsBurning;	}

	UFUNCTION()
		UParticleSystem* GetFireParticle() const; 
};
