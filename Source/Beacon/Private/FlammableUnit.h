// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "FlammableUnit.generated.h"

class UBoxComponent;
class UParticleSystemComponent;
class UFlammableUnit;

UENUM()
enum class ConnectType : uint8
{
	None = 1,
	SixDirection = 6,
	TwentySixDirection = 26
};

class Neighbors
{
public:
	Neighbors(ConnectType type, UFlammableUnit* self)
		:self(self), type(type)
	{}
	virtual ~Neighbors();
	virtual void SetNeighbor(int x, int y, int z, UFlammableUnit* unit);
	virtual int GetIndex(int x, int y, int z) = 0;

	template<typename T>
	static T* CreateNeighbors(UFlammableUnit* self)
	{
		return new T(self);
	}

public:
	UFlammableUnit* self;
	const ConnectType type;
	TArray<UFlammableUnit*> neighbors;
};
class SixDirNeighbors : public Neighbors
{
public:
	SixDirNeighbors(UFlammableUnit* self)
		:Neighbors(ConnectType::SixDirection, self)
	{
		neighbors.Init(nullptr, uint8(ConnectType::SixDirection));
	}
	virtual int GetIndex(int x, int y, int z) override;
};
class TwentySixDirNeighbors : public Neighbors
{
public:
	TwentySixDirNeighbors(UFlammableUnit* self)
		:Neighbors(ConnectType::TwentySixDirection, self)
	{
		neighbors.Init(nullptr, uint8(ConnectType::TwentySixDirection));
	}
	virtual int GetIndex(int x, int y, int z) override;
};

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

	void DisplayDebugInfo();

private:
	bool b_IsBurning;

	Neighbors* m_Neighbors;

public:
	UFUNCTION(BlueprintCallable)
		inline bool IsBurning() const { return b_IsBurning; }
};
