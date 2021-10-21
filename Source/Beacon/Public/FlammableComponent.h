// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlammableUnit.h"
#include "Containers/Queue.h"
#include "FlammableComponent.generated.h"

class UParticleSystemComponent;
class UStaticMeshComponent;

USTRUCT(BlueprintType)
struct FUnitCount
{
	GENERATED_BODY()
public:
	FUnitCount() {}
	FUnitCount(uint32 x, uint32 y, uint32 z)
		:X(x), Y(y), Z(z)
	{
	}
	UPROPERTY(EditAnywhere)
		uint32 X;
	UPROPERTY(EditAnywhere)
		uint32 Y;
	UPROPERTY(EditAnywhere)
		uint32 Z;
};

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

	virtual void DestroyComponent(bool bPromoteChildren) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_InitializeWithFlame;

	UPROPERTY(EditAnywhere)
		UParticleSystem* T_FireParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<UFlammableUnit*> m_FlammableUnits;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TMap<FVector, UFlammableUnit*> m_FlammableUnitsMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector m_UnitExtent;

	UPROPERTY(EditAnywhere)
		FUnitCount m_UnitCount;

	UPROPERTY(EditAnywhere)
		uint32 m_Count = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ConnectType m_ConnectType = ConnectType::SixDirection;

private:
	void CreateBoxFlammableUnits(const class UBoxComponent* box);
	void CreateCapsuleFlammableUnits(const class UCapsuleComponent* capsule);
	void CreateSphereFlammableUnits(const class USphereComponent* sphere);

private:
		bool b_IsBurning;
		TQueue<UFlammableUnit*> m_BurningUnits;

public:
	UFUNCTION(BlueprintCallable)
		void Ignited(UParticleSystem* particle);

	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		inline bool IsBurning() const { return b_IsBurning;	}

	//return nullptr is not burning
	UFUNCTION()
		inline UParticleSystem* GetFireParticle() const { return T_FireParticle; }
};