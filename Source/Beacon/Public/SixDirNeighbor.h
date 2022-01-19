// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neighbor.h"
#include "SixDirNeighbor.generated.h"



/**
 * 
 */
UCLASS()
class BEACON_API USixDirNeighbor : public UNeighbor
{
	GENERATED_BODY()

public:
	USixDirNeighbor();
	virtual int GetIndex(int x, int y, int z) override;
};
