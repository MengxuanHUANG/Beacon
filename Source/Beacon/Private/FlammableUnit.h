// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Unit.h"
#include "BeaconConnection.h"

#include "FlammableUnit.generated.h"

class UBoxComponent;
class UParticleSystemComponent;

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

#ifdef BEACON_DEBUG
		UBoxComponent* m_DebugBox;
#endif

	UPROPERTY(EditAnyWhere)
		FVector m_UnitExtent;

public:
	virtual void Initialize(FVector extent, ConnectType type) override;

	virtual void Trigger(UParticleSystem* particle) override;

	virtual void SetNeighbor(int x, int y, int z, Unit* unit) override;

	virtual const TArray<Unit*>& GetNeighbors() const override;

	virtual void IGetName(FString& name) override;

	virtual void DisplayDebugInfo() override;

private:
	bool b_IsBurning;
	
	Neighbors<Unit>* m_Neighbors;

public:
	inline bool IsTriggered() const { return b_IsBurning; }
};
