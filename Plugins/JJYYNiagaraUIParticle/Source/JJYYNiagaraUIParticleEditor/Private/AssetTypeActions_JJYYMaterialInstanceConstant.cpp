// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AssetTypeActions_JJYYMaterialInstanceConstant.h"
#include "Editor/MaterialEditor/Public/MaterialEditorModule.h"


UJJYYMaterialInstanceConstantFactory::UJJYYMaterialInstanceConstantFactory(const class FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {
    SupportedClass = UJJYYMaterialInstanceConstant::StaticClass();
    bCreateNew = true;
    bEditAfterNew = true;
}

bool UJJYYMaterialInstanceConstantFactory::CanCreateNew() const
{
    return true;
}

FText UJJYYMaterialInstanceConstantFactory::GetDisplayName() const
{
    return FText::FromString(TEXT("JJYYMat"));
}

FString UJJYYMaterialInstanceConstantFactory::GetDefaultNewAssetName() const
{
    return TEXT("NewJJYYMat");
}

UObject* UJJYYMaterialInstanceConstantFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    check(Class->IsChildOf(UJJYYMaterialInstanceConstant::StaticClass()));
    auto MIC = NewObject<UJJYYMaterialInstanceConstant>(InParent, Class, Name, Flags);
    if (MIC)
    {
        MIC->InitResources();

        if (InitialParent)
        {
            MIC->SetParentEditorOnly(InitialParent);
        }
    }
    return MIC;
}





void FAssetTypeActions_JJYYMaterialInstanceConstant::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor /*= TSharedPtr<IToolkitHost>()*/)
{
    EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

    for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
    {
        auto MIC = Cast<UMaterialInstanceConstant>(*ObjIt);
        if (MIC != NULL)
        {
            IMaterialEditorModule* MaterialEditorModule = &FModuleManager::LoadModuleChecked<IMaterialEditorModule>("MaterialEditor");
            MaterialEditorModule->CreateMaterialInstanceEditor(Mode, EditWithinLevelEditor, MIC);
        }
    }
}
