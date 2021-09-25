// Copyright Epic Games, Inc. All Rights Reserved.

#include "JJYYNiagaraUIParticleEditor.h"
#include "JJYYContentBrowserExtension.h"
#include "Developer/AssetTools/Public/AssetToolsModule.h"
#include "AssetTypeActions_JJYYMaterialInstanceConstant.h"

#define LOCTEXT_NAMESPACE "FJJYYNiagaraUIParticleEditorModule"

void FJJYYNiagaraUIParticleEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	if (!IsRunningCommandlet())
	{
		FJJYYContentBrowserExtension::InstallHooks();
	}

	IAssetTools& myAssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	myAssetTools.RegisterAssetTypeActions(MakeShareable(new FAssetTypeActions_JJYYMaterialInstanceConstant()));
}

void FJJYYNiagaraUIParticleEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	if (!IsRunningCommandlet())
	{
		FJJYYContentBrowserExtension::RemoveHooks();
	}

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FJJYYNiagaraUIParticleEditorModule, JJYYNiagaraUIParticleEditor)