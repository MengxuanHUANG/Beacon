// Copyright Epic Games, Inc. All Rights Reserved.

#include "Beacon.h"
#include "BeaconLog.h"

#include "FlammableComponent.h"

#include "BeaconEditorCommands.h"
#include "LevelEditor.h"
#include "Kismet/GameplayStatics.h"

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
	TArray<AActor*> actors;

	if (GEngine)
	{
		UGameplayStatics::GetAllActorsWithTag(GEditor->GetEditorWorldContext().World(), FName(BEACON_FLAMMABLE_TAG), actors);
		
		BEACON_LOG(Display, "Fund %d Actor", actors.Num());
		
		for (AActor* actor : actors)
		{
			FString name;
			actor->GetName(name);
			BEACON_LOG(Display, "address %s", *name);

			for (auto component : actor->GetComponentsByClass(UFlammableComponent::StaticClass()))
			{
				UFlammableComponent* flammable = Cast<UFlammableComponent>(component);
				flammable->ClearUnits();
				flammable->CreateUnits();
			}
		}
	}
}

void FBeaconModule::AddToolbarButton(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(BeaconEditorCommands::Get().m_BuildCommand);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBeaconModule, Beacon)