// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class USceneComponent;

class BEACON_API UnitsManager
{
public:
	UnitsManager() {}
	~UnitsManager() {}

	virtual void CreateUnits(const USceneComponent* component) = 0;
	//virtual void GetUnit(FVector index) = 0;
};