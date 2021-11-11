// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxUnitManagerComponent.h"
#include "BeaconLog.h"

void UBoxUnitManagerComponent::SetParameter3(uint32 x, uint32 y, uint32 z)
{
	BEACON_LOG(Warning, "Set 4");
	m_UnitCount.X = x;
	m_UnitCount.Y = y;
	m_UnitCount.Z = z;
}