// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifdef BEACON_DEBUG
	#undef BEACON_DEBUG
	#undef BEACON_ASSERT
#endif

#if !UE_BUILD_SHIPPING
	#ifndef BEACON_DEBUG
		#define BEACON_DEBUG
		#define BEACON_ASSERT(Statement) check(Statement);
	#endif

#endif

#ifdef BEACON_TAGS
	#undef BEACON_TAGS
	#undef BEACON_BUILDABLE_TAG
	#undef BEACON_FLAMMABLE_TAG
	#undef BEACON_FLAMMABLE_UNIT_TAG
#endif

#ifndef BEACON_TAGS
#define BEACON_TAGS
#define BEACON_BUILDABLE_TAG "Beacon_Buildable"
#define BEACON_FLAMMABLE_TAG "Beacon_Flammable"
#define BEACON_FLAMMABLE_UNIT_TAG "Beacon_FlammableUnit"
#endif