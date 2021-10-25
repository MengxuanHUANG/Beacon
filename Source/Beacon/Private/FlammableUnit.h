// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Unit.h"
#include "BeaconConnection.h"
#include "Components/SceneComponent.h"
#include "FlammableUnit.generated.h"

class UBoxComponent;
class UParticleSystemComponent;
class UFlammableUnit;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UFlammableUnit : public USceneComponent, public Unit
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
		UParticleSystemComponent* m_ParticleSystem;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly)
		UBoxComponent* m_DebugBox;

	UPROPERTY(EditAnyWhere)
		FVector m_UnitExtent;

public:
	virtual void Initialize(FVector extent, ConnectType type) override;

	virtual void OnDestroy(bool bPromoteChildren) override;

	virtual void Ignite(UParticleSystem* particle) override;

	virtual void SetNeighbor(int x, int y, int z, Unit* unit) override;

	virtual const TArray<Unit*>& GetNeighbors() const override;

	virtual void IGetName(FString& name) override;
	
	//TODO: remove
	UFUNCTION()
		void IncreaseTemperature(float temperature);
	inline  float GetTemperature() const { return m_Temperature; }
	
	virtual void DisplayDebugInfo() override;
private:
	UPROPERTY(VisibleAnywhere)
		float m_Temperature = 0;

	bool b_IsBurning;

	Neighbors<Unit>* m_Neighbors;

public:
	UFUNCTION(BlueprintCallable)
		inline bool IsBurning() const { return b_IsBurning; }
};
