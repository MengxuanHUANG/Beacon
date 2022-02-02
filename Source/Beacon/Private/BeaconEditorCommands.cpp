// Fill out your copyright notice in the Description page of Project Settings.

#include "BeaconEditorCommands.h"
#include "Beacon.h"

#define LOCTEXT_NAMESPACE "FBeaconModule"

void BeaconEditorCommands::RegisterCommands()
{
	UI_COMMAND(m_ClearCommand, "BeaconClear", "BeaconClearCommand", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(m_BuildCommand, "BeaconBuild", "BeaconBuildCommand", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(m_RebuildCommand, "BeaconRebuild", "BeaconRebuildCommand", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE