// Fill out your copyright notice in the Description page of Project Settings.


#include "Neighbor.h"

// Sets default values for this component's properties
UNeighbor::UNeighbor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UNeighbor::SetNeighbor(int x, int y, int z, UUnitComponent* unit)
{
	int index = GetIndex(x + 1, y + 1, z + 1);
	neighbors[index] = unit;
}