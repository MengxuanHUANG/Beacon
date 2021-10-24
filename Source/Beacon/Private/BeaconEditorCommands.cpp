// Fill out your copyright notice in the Description page of Project Settings.

#include "BeaconEditorCommands.h"
#include "Beacon.h"

#define LOCTEXT_NAMESPACE "FBeaconModule"

void BeaconEditorCommands::RegisterCommands()
{
	UI_COMMAND(m_BuildCommand, "BeaconBuildCommand", "BeaconBuildCommand Units", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE