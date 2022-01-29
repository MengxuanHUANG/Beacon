// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BeaconCore.h"
#include "Logging/LogMacros.h"
#include "Containers/UnrealString.h"



/*
* Beacon Log macro
* Example: BEACON_LOG(Display, "Message", Parameters);
*/
#ifdef BEACON_LOG_FULL
#undef BEACON_LOG_FULL
#endif

#ifdef BEACON_LOG
#undef BEACON_LOG
#endif

#ifndef BEACON_LOG

#define CAT(a, b) a##b

#ifdef BEACON_DEBUG	
	DECLARE_LOG_CATEGORY_EXTERN(BeaconLog, Log, All);

	#define BEACON_LOG_FULL(Verbosity, Format, ...) \
		UE_LOG(BeaconLog, Verbosity, TEXT(CAT("Line %d in %s(%s)\n", Format)), __LINE__, *FString(__FILE__), *FString(__FUNCTION__), __VA_ARGS__)
	
	#define BEACON_LOG(Verbosity, Format, ...) \
		UE_LOG(BeaconLog, Verbosity, TEXT(Format), __VA_ARGS__)
#else
	#define BEACON_LOG_FULL(Verbosity, Format, ...)
	#define BEACON_LOG(Verbosity, Format, ...)
#endif
#endif