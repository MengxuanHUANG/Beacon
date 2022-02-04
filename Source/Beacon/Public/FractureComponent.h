// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildableComponent.h"
#include "FractureComponent.generated.h"

class UGeometryCollectionComponent;
class UBoxComponent;
class UDebrisUnitManagerComponent;
class UFractureMaterial;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFractured);

USTRUCT(BlueprintType)
struct FFragment
{
	GENERATED_BODY()
public:
	FFragment() {}
	FFragment(int32 index, int32 level = 0)
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

	bool InitializeCurrentDebris();
	void ClearCurrentDebris();
	void UpdateCurrentDebris();

public:
	UFUNCTION()
		inline TArray<FFragment>& GetCurrentFragments() { return m_CurrentFragments; }
	UFUNCTION()
		inline TArray<FVector>& GetFragmentsWorldLocation() { return m_FragmentWorldLocation; }
	UFUNCTION()
		inline TArray<FFragment>& GetRemovedFragments() { return m_RemovedFragments; }
	UFUNCTION()
		inline TArray<FFragment>& GetNeedRemoveFragments() { return m_NeedRemoveFragments; }
public:
	UPROPERTY(BlueprintAssignable)
		FOnFractured OnFracturedEvent;

	UPROPERTY(EditAnywhere)
		UFractureMaterial* T_FractureMaterial;

	UPROPERTY(EditAnywhere)
		UGeometryCollectionComponent* GeometryCollectionComponent;

	//Array of debris' Need to be Removed in last update, will be empty after calling UpdateCurrentDebris
	UPROPERTY(VisibleAnywhere)
		TArray<FFragment> m_NeedRemoveFragments;

	//Array of removed debris' index in last update
	UPROPERTY(VisibleAnywhere)
		TArray<FFragment> m_RemovedFragments;

	//Array of pairs that store debris' index and level 
	UPROPERTY(VisibleAnywhere)
		TArray<FFragment> m_CurrentFragments;
	
	//Array of debris' world location
	UPROPERTY(VisibleAnywhere)
		TArray<FVector> m_FragmentWorldLocation;

	UPROPERTY(VisibleAnywhere)
		UDebrisUnitManagerComponent* m_DebrisUnitManager;
private:
	bool CheckBreak();

private:
	UPROPERTY(VisibleAnywhere)
		bool bIsBreak;
};
