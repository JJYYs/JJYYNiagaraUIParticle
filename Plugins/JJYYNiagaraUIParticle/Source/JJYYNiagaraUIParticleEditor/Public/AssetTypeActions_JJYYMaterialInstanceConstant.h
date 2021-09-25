// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "JJYYMaterialInstanceConstant.h"
#include "UnrealED/Classes/Factories/Factory.h"
#include "Developer/AssetTools/Public/AssetTypeCategories.h"
#include "Developer/AssetTools/Public/AssetTypeActions/AssetTypeActions_ClassTypeBase.h"
#include "AssetTypeActions_JJYYMaterialInstanceConstant.generated.h"



UCLASS()
class UJJYYMaterialInstanceConstantFactory : public UFactory
{
    GENERATED_UCLASS_BODY()
    virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
    virtual bool CanCreateNew() const override;

    UPROPERTY()
        class UMaterialInterface* InitialParent;
public:
    FText GetDisplayName() const override;


    FString GetDefaultNewAssetName() const override;

};


class FAssetTypeActions_JJYYMaterialInstanceConstant : public FAssetTypeActions_Base
{
public:

    // IAssetTypeActions Implementation
    virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_JJYYMaterialInstanceConstant", "JJYY Material Instance"); }
    virtual FColor GetTypeColor() const override { return FColor(200, 0, 0); }
    virtual UClass* GetSupportedClass() const override { return UJJYYMaterialInstanceConstant::StaticClass(); }
    //virtual void GetActions(const TArray<UObject*>& InObjects, struct FToolMenuSection& Section) override;
    virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
    virtual bool CanFilter() override { return true; }
    virtual uint32 GetCategories() override { return EAssetTypeCategories::MaterialsAndTextures; }

private:
    /** Handler for when FindParent is selected */
    //void ExecuteFindParent(TArray<TWeakObjectPtr<UMaterialInstanceConstant>> Objects);

    //EAssetTypeCategories::Type AssetCategoryBit;
};