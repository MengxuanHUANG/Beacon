// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct BeaconThermalData
{
	bool bIsBurning;
	FVector Direction;
	float Thermal_Value;
};

class BEACON_API BeaconThermalProxy
{
public:
	virtual ~BeaconThermalProxy() = default;

public:
	virtual void UpdateThermalData() {}
	TSharedPtr<BeaconThermalData>& GetBeaconThermalData() { return ThermalData; }

	inline bool GetThermalProxyNeedUpdate() const { return bNeedUpdateThermalData; }
	void SetThermalProxyNeedUpdate(bool needUpdate);

protected:
	TSharedPtr<BeaconThermalData> ThermalData;

private:
	bool bNeedUpdateThermalData = false;
};
