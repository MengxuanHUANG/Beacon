// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#if !UE_BUILD_SHIPPING
	#ifndef BEACON_DEBUG
		#define BEACON_DEBUG
		#define BEACON_ASSERT(Statement) check(Statement);
	#endif

#endif

#ifndef BEACON_TAGS
#define BEACON_TAGS
#define BEACON_FLAMMABLE_TAG "Beacon_Flammable"
#define BEACON_FLAMMABLE_UNIT_TAG "Beacon_FlammableUnit"
#endif