// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#if !UE_BUILD_SHIPPING
	#ifndef BEACON_DEBUG
		#define BEACON_DEBUG
		#define BEACON_ASSERT(Statement) check(Statement);
	#endif

	//Whether to hide box for UnitComponent
	#define BEACON_HIDE_DEBUG_BOX true
	#define BEACON_HIDE_DEBUG_BOX_IN_GAME false

#endif

#define BEACON_FLAMMABLE_TAG "Beacon_Flammable"