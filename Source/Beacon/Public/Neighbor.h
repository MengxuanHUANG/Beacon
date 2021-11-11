// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Neighbor.generated.h"


class UUnitComponent;

UENUM()
enum class ConnectType : uint8
{
	None = 0,
	SixDirection = 6,
	TwentySixDirection = 26
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEACON_API UNeighbor : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNeighbor();
	virtual ~UNeighbor() = default;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	
	virtual void SetNeighbor(int x, int y, int z, UUnitComponent* unit);
	virtual int GetIndex(int x, int y, int z) { return -1; }

public:
	UUnitComponent* self;
	ConnectType type;

	UPROPERTY(VisibleAnywhere)
		TArray<UUnitComponent*> neighbors;

public:
	template<typename T>
	static T* CreateNeighbors(UUnitComponent* self)
	{
		return new T(self);
	}
};
