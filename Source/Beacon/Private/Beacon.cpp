// Copyright Epic Games, Inc. All Rights Reserved.

#include "Beacon.h"
#include "BeaconLog.h"

#include "BuildableComponent.h"

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
	}
}

void FBeaconModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	BeaconEditorCommands::Unregister();
}

void FBeaconModule::Build()
{
	if (GEngine)
	{
		TArray<AActor*> actors;

		//obtain actors with tag
		UGameplayStatics::GetAllActorsWithTag(GEditor->GetEditorWorldContext().World(), FName(BEACON_BUILDABLE_TAG), actors);
		
		BEACON_LOG(Display, "Fund %d Actor with FlammableComponent", actors.Num());
		
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
}
void FBeaconModule::Clear()
{
	if (GEngine)
	{
		TArray<AActor*> actors;

		//obtain actors with tag
		UGameplayStatics::GetAllActorsWithTag(GEditor->GetEditorWorldContext().World(), FName(BEACON_BUILDABLE_TAG), actors);

		BEACON_LOG(Display, "Fund %d Actor with FlammableComponent", actors.Num());

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
}
void FBeaconModule::Rebuild()
{
	if (GEngine)
	{
		TArray<AActor*> actors;

		//obtain actors with tag
		UGameplayStatics::GetAllActorsWithTag(GEditor->GetEditorWorldContext().World(), FName(BEACON_BUILDABLE_TAG), actors);

		BEACON_LOG(Display, "Fund %d Actor with FlammableComponent", actors.Num());

		for (AActor* actor : actors)
		{
			//actor->GetComponentsByInterface
			for (auto component : actor->GetComponentsByClass(UBuildableComponent::StaticClass()))
			{
				UBuildableComponent* build = Cast<UBuildableComponent>(component);
				build->Clear();
				build->Build();
			}
		}
	}
}

void FBeaconModule::AddToolbarButton(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(BeaconEditorCommands::Get().m_ClearCommand);
	Builder.AddToolBarButton(BeaconEditorCommands::Get().m_BuildCommand);
	Builder.AddToolBarButton(BeaconEditorCommands::Get().m_RebuildCommand);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBeaconModule, Beacon)