// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BeaconCore.h"
#include "Logging/LogMacros.h"
#include "Containers/UnrealString.h"

DECLARE_LOG_CATEGORY_EXTERN(BeaconLog, Log, All);
DEFINE_LOG_CATEGORY(BeaconLog);

#define CAT(a, b) a##b

/*
* Beacon Log macro
* Example: BEACON_LOG(Display, "Message", Parameters);
*/
#ifndef BEACON_LOG
#ifdef BEACON_DEBUG
	#define BEACON_LOG(Verbosity, Format, ...) \
		UE_LOG(BeaconLog, Verbosity, TEXT(CAT("Line %d in %s(%s)\n", Format)), __LINE__, *FString(__FILE__), *FString(__FUNCTION__), __VA_ARGS__)
#else
	#define BEACON_LOG(Verbosity, Format, ...)
#endif
#endif