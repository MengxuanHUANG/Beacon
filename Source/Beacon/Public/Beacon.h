// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;

class FBeaconModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void BuildUnits();
	void AddToolbarButton(FToolBarBuilder& Builder);
private:
	TSharedPtr<FUICommandList> m_BeaconCommands;
};
