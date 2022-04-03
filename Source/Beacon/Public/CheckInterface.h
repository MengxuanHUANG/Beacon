// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CheckInterface.generated.h"

#ifndef BEACON_CHECK_MACRO
#define BEACON_CHECK_MACRO
#define BEACON_CLEAR_FSTRING(str) str.Empty();
#define BEACON_AUTO_FILL_INFO(info) info.Append("File: "); \
		info.Append(__FILE__);\
		info.Append("; Function: ");\
		info.Append(__FUNCTION__);\
		info.Append("; Line ");\
		info.AppendInt(__LINE__);
#endif

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UCheckInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BEACON_API ICheckInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool BeaconCheck(FString& Info, FString& Message);
};
