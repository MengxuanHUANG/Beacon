// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LateUpdateComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FLateUpdateFunction, float, DeltaTime);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEACON_API ULateUpdateComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULateUpdateComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FLateUpdateFunction LateTickFunction;

public:
	/*
	* Create & Register a ULateUpdateComponent at runtime
	*/
	static ULateUpdateComponent* CreateLateUpdataComponent(UObject* Outer);
};
