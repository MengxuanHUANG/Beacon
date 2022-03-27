// Fill out your copyright notice in the Description page of Project Settings.


#include "ThermalRadiationMaterial.h"

UThermalRadiationMaterial::UThermalRadiationMaterial()
{
	Distance_Curve = CreateDefaultSubobject<UCurveFloat>(TEXT("Curve"));
}

float UThermalRadiationMaterial::GetThermalTransmitValue(float distance) const
{
	return (EnableAttenuation
		&& distance > Attenuation_Start_Radius) ?
		Distance_Curve->GetFloatValue(distance) : Thermal_Transmit;
}