// Fill out your copyright notice in the Description page of Project Settings.


#include "BeaconMaterial.h"
#include "Curves/CurveFloat.h"

UBeaconMaterial::UBeaconMaterial()
{
	Temperature_Curve = CreateDefaultSubobject<UCurveFloat>(TEXT("Curve"));
}

ObjectTemplate UBeaconMaterial::GetObjectTemplate() const
{
	return Template;
}