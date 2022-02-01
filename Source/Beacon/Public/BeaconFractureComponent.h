// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "BeaconFractureComponent.generated.h"


class UGeometryCollectionComponent;
class UBoxComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEACON_API UBeaconFractureComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBeaconFractureComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere)
		UGeometryCollectionComponent* GeometryCollectionComponent;

	//Array of pairs that store debris' index and level 
	TArray<TPair<int32, int32> > m_CurrentDebrisIndex;

	UPROPERTY(EditAnywhere)
		int size;

	UPROPERTY(EditAnywhere)
		FColor color;

	UPROPERTY(EditAnywhere)
		bool draw;

	UPROPERTY(VisibleAnyWhere)
		TArray<UBoxComponent*> m_Boxes;

protected:
	void InitializeCurrentDebris();
	void UpdateCurrentDebris();
};
