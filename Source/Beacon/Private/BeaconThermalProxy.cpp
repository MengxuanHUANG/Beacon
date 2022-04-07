// Fill out your copyright notice in the Description page of Project Settings.


#include "BeaconThermalProxy.h"

void BeaconThermalProxy::SetThermalProxyNeedUpdate(bool needUpdate)
{
	if (needUpdate)
	{
		if (!ThermalData.IsValid())
		{
			ThermalData = MakeShared<BeaconThermalData>();
		}
	}
	else if (ThermalData.IsValid())
	{
		ThermalData.Reset();
	}
	bNeedUpdateThermalData = needUpdate;
}