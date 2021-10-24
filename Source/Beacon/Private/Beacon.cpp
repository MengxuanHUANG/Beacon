// Copyright Epic Games, Inc. All Rights Reserved.

#include "Beacon.h"
#include "BeaconEditorCommands.h"
#include "LevelEditor.h"

#define LOCTEXT_NAMESPACE "FBeaconModule"

void FBeaconModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	BeaconEditorCommands::Register();

	m_BeaconCommands = MakeShareable(new FUICommandList);
	m_BeaconCommands->MapAction(
		BeaconEditorCommands::Get().m_BuildCommand,
		FExecuteAction::CreateRaw(this, &FBeaconModule::BuildUnits)
	);

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	{
		TSharedPtr<FExtender> NewToolbarExtender = MakeShareable(new FExtender);
		NewToolbarExtender->AddToolBarExtension("Content",
			EExtensionHook::Before,
			m_BeaconCommands,
			FToolBarExtensionDelegate::CreateRaw(this, &FBeaconModule::AddToolbarButton));

		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(NewToolbarExtender);
	}
}

void FBeaconModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	BeaconEditorCommands::Unregister();
}

void FBeaconModule::BuildUnits()
{
	UE_LOG(LogTemp, Warning, TEXT("Click"));
}

void FBeaconModule::AddToolbarButton(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(BeaconEditorCommands::Get().m_BuildCommand);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBeaconModule, Beacon)