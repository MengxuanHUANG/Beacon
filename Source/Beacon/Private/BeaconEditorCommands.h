// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SlateBasics.h"
#include "EditorStyleSet.h"

/**
 * 
 */
class BeaconEditorCommands : public TCommands<BeaconEditorCommands>
{
public:

	BeaconEditorCommands()
		: TCommands<BeaconEditorCommands>
		(
			TEXT("BeaconEditor"),
			NSLOCTEXT("Contexts", "BeaconEditor", "BeaconEditor Plugin"),
			NAME_None,
			FEditorStyle::GetStyleSetName()
			) {}

	virtual void RegisterCommands() override;

public:

	TSharedPtr<FUICommandInfo> m_BuildCommand;
};
