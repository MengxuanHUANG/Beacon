// Fill out your copyright notice in the Description page of Project Settings.


#include "SixDirNeighbor.h"

USixDirNeighbor::USixDirNeighbor()
{
	neighbors.Init(nullptr, uint8(ConnectType::SixDirection));
}

int USixDirNeighbor::GetIndex(int x, int y, int z)
{
	return ((x & 2) >> 1)
		+ ((y ^ 1) & 1) * 2 + ((y & 2) >> 1)
		+ ((z ^ 1) & 1) * 4 + ((z & 2) >> 1);
}