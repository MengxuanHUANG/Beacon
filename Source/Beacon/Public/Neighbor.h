// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Neighbor.generated.h"


#ifndef BEACON_SIX_DIR_MACRO
	#define BEACON_SIX_DIR_MACRO
	#define BEACON_SIX_DIR_X_FORWARD  FVector( 1, 0, 0)
	#define BEACON_SIX_DIR_X_BACKWARD FVector(-1, 0, 0)
	#define BEACON_SIX_DIR_Y_FORWARD  FVector(0,  1, 0)
	#define BEACON_SIX_DIR_Y_BACKWARD FVector(0, -1, 0)
	#define BEACON_SIX_DIR_Z_FORWARD  FVector(0, 0,  1)
	#define BEACON_SIX_DIR_Z_BACKWARD FVector(0, 0, -1)
#endif

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
	virtual int GetCount() const { return 0; }

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
