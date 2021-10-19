// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BeaconConnection.h"
#include "Components/SceneComponent.h"
#include "FlammableUnit.generated.h"

class UBoxComponent;
class UParticleSystemComponent;
class UFlammableUnit;

//UENUM()
//enum class ConnectType : uint8
//{
//	None = 1,
//	SixDirection = 6,
//	TwentySixDirection = 26
//};

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

	UPROPERTY(EditAnyWhere)
		ConnectType m_ConnectType;

public:
	UFUNCTION(BlueprintCallable)
		void Initialize(FVector extent, ConnectType type);

	UFUNCTION(BlueprintCallable)
		void UpdateExtent(FVector extent);

	UFUNCTION(BlueprintCallable)
		void Ignite(UParticleSystem* particle);

	UFUNCTION(BlueprintCallable)
		void SetNeighbor(int x, int y, int z, UFlammableUnit* unit);

	UFUNCTION(BlueprintCallable)
		const TArray<UFlammableUnit*>& GetNeighbors() const;

	//TODO: remove
	UFUNCTION()
		void IncreaseTemperature(float temperature);

	inline  float GetTemperature() const { return m_Temperature; }
	
	void DisplayDebugInfo();

private:
	UPROPERTY(VisibleAnywhere)
		float m_Temperature = 0;

	bool b_IsBurning;

	Neighbors<UFlammableUnit>* m_Neighbors;

public:
	UFUNCTION(BlueprintCallable)
		inline bool IsBurning() const { return b_IsBurning; }
};
