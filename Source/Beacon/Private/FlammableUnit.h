// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "FlammableUnit.generated.h"

class UBoxComponent;
class UParticleSystemComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UFlammableUnit : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFlammableUnit();

	virtual void DestroyComponent(bool bPromoteChildren) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly)
		UBoxComponent* m_DebugBox;
	
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly)
		UParticleSystemComponent* m_ParticleSystem;

	UPROPERTY(EditAnyWhere)
		FVector m_UnitExtent;

public:
	UFUNCTION(BlueprintCallable)
		void Initialize(FVector extent);

	UFUNCTION(BlueprintCallable)
		void UpdateExtent(FVector extent);

	UFUNCTION(BlueprintCallable)
		void Ignite(UParticleSystem* particle);

private:
	bool b_IsBurning;

public:
	UFUNCTION(BlueprintCallable)
		inline bool IsBurning() const { return b_IsBurning; }
};
