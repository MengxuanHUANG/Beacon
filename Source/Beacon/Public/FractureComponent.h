// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildableComponent.h"
#include "FractureComponent.generated.h"

class UGeometryCollectionComponent;
class UBoxComponent;

USTRUCT(BlueprintType)
struct FFragment
{
	GENERATED_BODY()
public:
	FFragment() {}
	FFragment(int32 index, int32 level)
		:Index(index), Level(level)
	{}
	FFragment(const FFragment& other)
		:Index(other.Index), Level(other.Level)
	{}

	
	UPROPERTY(VisibleAnywhere)
		int32 Index;
	UPROPERTY(VisibleAnywhere)
		int32 Level;

public:
	//Begin necessary operator for TArray
	bool operator == (const FFragment& other) const
	{
		return Index == other.Index;
	}
	//End necessary operator for TArray
};

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEACON_API UFractureComponent : public UBuildableComponent
{
	GENERATED_BODY()
public:
	UFractureComponent();
public:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	//Begin declaring BuildableComponent functions
	virtual bool Build_Implement() override;
	virtual	void Clear_Implement() override;
	//End declaring BuildableComponent functions

protected:
	bool InitializeCurrentDebris();
	void ClearCurrentDebris();
	void UpdateCurrentDebris();

public:
	UPROPERTY(EditAnywhere)
		UGeometryCollectionComponent* GeometryCollectionComponent;

	//Array of pairs that store debris' index and level 
	UPROPERTY(VisibleAnywhere)
		TArray<FFragment> m_CurrentFragments;

	//TODO: remove
	UPROPERTY(EditAnywhere)
		int size;

	UPROPERTY(EditAnywhere)
		FColor color;

	UPROPERTY(VisibleAnyWhere)
		TArray<UBoxComponent*> m_Boxes;
};
