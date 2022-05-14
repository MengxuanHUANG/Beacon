// Copyright Epic Games, Inc. All Rights Reserved.

#include "Beacon.h"
#include "BeaconLog.h"

#include "BuildableComponent.h"

#ifdef BEACON_DEBUG
#include "LevelEditor.h"
#endif

#include "Kismet/GameplayStatics.h"

#define LOCTEXT_NAMESPACE "FBeaconModule"

void FBeaconModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	/*BeaconEditorCommands::Register();

	m_BeaconCommands = MakeShareable(new FUICommandList);
	
	m_BeaconCommands->MapAction(
		BeaconEditorCommands::Get().m_ClearCommand,
		FExecuteAction::CreateRaw(this, &FBeaconModule::Clear)
	);
	m_BeaconCommands->MapAction(
		BeaconEditorCommands::Get().m_BuildCommand,
		FExecuteAction::CreateRaw(this, &FBeaconModule::Build)
	);
	m_BeaconCommands->MapAction(
		BeaconEditorCommands::Get().m_RebuildCommand,
		FExecuteAction::CreateRaw(this, &FBeaconModule::Rebuild)
	);
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	{
		TSharedPtr<FExtender> NewToolbarExtender = MakeShareable(new FExtender);
		NewToolbarExtender->AddToolBarExtension("Content",
			EExtensionHook::Before,
			m_BeaconCommands,
			FToolBarExtensionDelegate::CreateRaw(this, &FBeaconModule::AddToolbarButton));

		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(NewToolbarExtender);
	}*/
}

void FBeaconModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	//BeaconEditorCommands::Unregister();
}

void FBeaconModule::Build()
{
#ifdef BEACON_DEBUG

	if (GEngine)
	{
		TArray<AActor*> actors;

		//obtain actors with tag
		UGameplayStatics::GetAllActorsWithTag(GEditor->GetEditorWorldContext().World(), FName(BEACON_BUILDABLE_TAG), actors);
		
		for (AActor* actor : actors)
		{
			//actor->GetComponentsByInterface
			for (auto component : actor->GetComponentsByClass(UBuildableComponent::StaticClass()))
			{
				UBuildableComponent* build = Cast<UBuildableComponent>(component);
				build->Build();
			}
		}
	}

#endif // BEACON_DEBUG
}
void FBeaconModule::Clear()
{
#ifdef BEACON_DEBUG
	if (GEngine)
	{
		TArray<AActor*> actors;

		//obtain actors with tag
		UGameplayStatics::GetAllActorsWithTag(GEditor->GetEditorWorldContext().World(), FName(BEACON_BUILDABLE_TAG), actors);

		for (AActor* actor : actors)
		{
			//actor->GetComponentsByInterface
			for (auto component : actor->GetComponentsByClass(UBuildableComponent::StaticClass()))
			{
				UBuildableComponent* build = Cast<UBuildableComponent>(component);
				build->Clear();
			}
		}
	}
#endif // BEACON_DEBUG
}
void FBeaconModule::Rebuild()
{
#ifdef BEACON_DEBUG
	if (GEngine)
	{
		TArray<AActor*> actors;

		//obtain actors with tag
		UGameplayStatics::GetAllActorsWithTag(GEditor->GetEditorWorldContext().World(), FName(BEACON_BUILDABLE_TAG), actors);

		for (AActor* actor : actors)
		{
			//actor->GetComponentsByInterface
			for (auto component : actor->GetComponentsByClass(UBuildableComponent::StaticClass()))
			{
				UBuildableComponent* build = Cast<UBuildableComponent>(component);
				build->ReBuild();
			}
		}
	}
#endif // BEACON_DEBUG
}

void FBeaconModule::AddToolbarButton(FToolBarBuilder& Builder)
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBeaconModule, Beacon)