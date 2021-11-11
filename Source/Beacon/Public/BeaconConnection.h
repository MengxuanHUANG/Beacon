// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Neighbor.h"

/**
 *
 */

template<typename Unit>
class Neighbors
{
public:
	Neighbors(ConnectType type, Unit* self)
		:self(self), type(type)
	{}
	virtual ~Neighbors() = default;
	virtual void SetNeighbor(int x, int y, int z, Unit* unit)
	{
		int index = GetIndex(x + 1, y + 1, z + 1);
		neighbors[index] = unit;
	}
	virtual int GetIndex(int x, int y, int z) = 0;

	template<typename T>
	static T* CreateNeighbors(Unit* self)
	{
		return new T(self);
	}

public:
	Unit* self;
	const ConnectType type;
	TArray<Unit*> neighbors;
};

template<typename Unit>
class SixDirNeighbors : public Neighbors<Unit>
{
public:
	SixDirNeighbors(Unit* self)
		:Neighbors<Unit>(ConnectType::SixDirection, self)
	{
		neighbors.Init(nullptr, uint8(ConnectType::SixDirection));
	}
	virtual int GetIndex(int x, int y, int z) override
	{
		return ((x & 2) >> 1)
			+ ((y ^ 1) & 1) * 2 + ((y & 2) >> 1)
			+ ((z ^ 1) & 1) * 4 + ((z & 2) >> 1);
	}
};

template<typename Unit>
class TwentySixDirNeighbors : public Neighbors<Unit>
{
public:
	TwentySixDirNeighbors(Unit* self)
		:Neighbors<Unit>(ConnectType::TwentySixDirection, self)
	{
		neighbors.Init(nullptr, uint8(ConnectType::TwentySixDirection));
	}
	virtual int GetIndex(int x, int y, int z) override
	{
		return x + y * 3 + z * 9;
	}
};