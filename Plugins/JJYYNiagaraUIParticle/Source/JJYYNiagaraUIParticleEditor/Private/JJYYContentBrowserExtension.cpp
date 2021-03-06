// Copyright (c) 2021 Michal Smoleň 


#include "JJYYContentBrowserExtension.h"
#include "ContentBrowserDelegates.h"
#include "ContentBrowserModule.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "MaterialEditingLibrary.h"
#include "MaterialGraph/MaterialGraph.h"
#include "Materials/MaterialExpressionVertexColor.h"
#include "Materials/MaterialExpressionParticleColor.h"
#include "IContentBrowserSingleton.h"
#include "AssetTypeActions_JJYYMaterialInstanceConstant.h"


#define LOCTEXT_NAMESPACE "JJYYNiagaraUIParticle"


static FContentBrowserMenuExtender_SelectedAssets ContentBrowserExtenderDelegate;
static FDelegateHandle ContentBrowserExtenderDelegateHandle;


struct FContentBrowserSelectedAssetExtensionBase
{
public:
	TArray<struct FAssetData> SelectedAssets;

public:
	virtual void Execute() {}
	virtual ~FContentBrowserSelectedAssetExtensionBase() {}
};

#include "IAssetTools.h"
#include "AssetToolsModule.h"

struct FCreateNiagaraUIMaterialsExtension : public FContentBrowserSelectedAssetExtensionBase
{
	void CreateNiagaraUIMaterials(TArray<UMaterial*>& Materials)
	{		
        FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
		FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

        const FString DefaultSuffix = TEXT("_JJYYInst");

        
		
		TArray<UObject*> ObjectsToSync;
		for (auto ObjIt = Materials.CreateConstIterator(); ObjIt; ++ObjIt)
		{
			auto Object = (*ObjIt);
			if (Object && Object->MaterialDomain == EMaterialDomain::MD_UI)
			{
				// Determine an appropriate name
				FString Name;
				FString PackageName;
                AssetToolsModule.Get().CreateUniqueAssetName(Object->GetOutermost()->GetName(), DefaultSuffix, PackageName, Name);

				// Create the factory used to generate the asset
				UJJYYMaterialInstanceConstantFactory* Factory = NewObject<UJJYYMaterialInstanceConstantFactory>();
				Factory->InitialParent = Object;

				UObject* NewAsset = AssetToolsModule.Get().CreateAsset(Name, FPackageName::GetLongPackagePath(PackageName), UJJYYMaterialInstanceConstant::StaticClass(), Factory);

				if (NewAsset)
				{
					ObjectsToSync.Add(NewAsset);
				}
			}
		}
        if (ObjectsToSync.Num() > 0)
        {
            ContentBrowserModule.Get().SyncBrowserToAssets(ObjectsToSync);
        }
	}

	virtual void Execute() override
	{
		TArray<UMaterial*> Materials;
		for (auto AssetIt = SelectedAssets.CreateConstIterator(); AssetIt; ++AssetIt)
		{
			const FAssetData& AssetData = *AssetIt;
			if (UMaterial* Material = Cast<UMaterial>(AssetData.GetAsset()))
			{
				Materials.Add(Material);
			}
		}

		CreateNiagaraUIMaterials(Materials);
	}
};

class FJJYYContentBrowserExtension_Impl
{
public:
	static void ExecuteSelectedContentFunctor(TSharedPtr<FContentBrowserSelectedAssetExtensionBase> SelectedAssetFunctor)
	{
		SelectedAssetFunctor->Execute();
	}

	static void CreateNiagaraUIFunctions(FMenuBuilder& MenuBuilder, TArray<FAssetData> SelectedAssets)
	{
		TArray<UMaterial*> Materials;
		for (auto AssetIt = SelectedAssets.CreateConstIterator(); AssetIt; ++AssetIt)
		{
			const FAssetData& AssetData = *AssetIt;
			if (UMaterial* Material = Cast<UMaterial>(AssetData.GetAsset()))
			{
				if (Material->MaterialDomain == EMaterialDomain::MD_UI)
				{
					Materials.Add(Material);
				}
			}
		}
		if (Materials.Num() == 0)
		{
			return;
		}

		TSharedPtr<FCreateNiagaraUIMaterialsExtension> NiagaraUIMaterialsFunctor = MakeShareable(new FCreateNiagaraUIMaterialsExtension());
		NiagaraUIMaterialsFunctor->SelectedAssets = SelectedAssets;

		FUIAction ActionCreateNiagaraUIMaterial(FExecuteAction::CreateStatic(&FJJYYContentBrowserExtension_Impl::ExecuteSelectedContentFunctor, StaticCastSharedPtr<FContentBrowserSelectedAssetExtensionBase>(NiagaraUIMaterialsFunctor)));
		
		MenuBuilder.AddMenuEntry(
			LOCTEXT("JJYYNiagaraUIParticle_CreateUIMaterial", "Create JJYY Material Instance"),
			LOCTEXT("JJYYNiagaraUIParticle_CreateUIMaterialTooltip", "Create a Material for UI and Niagara"),
			FSlateIcon(FEditorStyle::GetStyleSetName(), "ClassIcon.MaterialInstanceActor"),
			ActionCreateNiagaraUIMaterial,
			NAME_None,
			EUserInterfaceActionType::Button
		);
	}

	static TSharedRef<FExtender> OnExtendContentBrowserAssetSelectionMenu(const TArray<FAssetData>& SelectedAssets)
	{
		TSharedRef<FExtender> Extender(new FExtender());

		bool AnyMaterials = false;
		for (auto AssetIt = SelectedAssets.CreateConstIterator(); AssetIt; ++AssetIt)
		{
			const FAssetData& Asset = *AssetIt;
			AnyMaterials = AnyMaterials || (Asset.AssetClass == UMaterial::StaticClass()->GetFName());
		}

		if (AnyMaterials)
		{
			Extender->AddMenuExtension(
				"GetAssetActions",
				EExtensionHook::After,
				nullptr,
				FMenuExtensionDelegate::CreateStatic(&FJJYYContentBrowserExtension_Impl::CreateNiagaraUIFunctions, SelectedAssets));
		}

		return Extender;
	}

	static TArray<FContentBrowserMenuExtender_SelectedAssets>& GetExtenderDelegates()
	{
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
		return ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
	}
};

void FJJYYContentBrowserExtension::InstallHooks()
{
	ContentBrowserExtenderDelegate = FContentBrowserMenuExtender_SelectedAssets::CreateStatic(&FJJYYContentBrowserExtension_Impl::OnExtendContentBrowserAssetSelectionMenu);

	TArray<FContentBrowserMenuExtender_SelectedAssets>& CBMenuExtenderDelegates = FJJYYContentBrowserExtension_Impl::GetExtenderDelegates();
	CBMenuExtenderDelegates.Add(ContentBrowserExtenderDelegate);
	ContentBrowserExtenderDelegateHandle = CBMenuExtenderDelegates.Last().GetHandle();
}

void FJJYYContentBrowserExtension::RemoveHooks()
{
	TArray<FContentBrowserMenuExtender_SelectedAssets>& CBMenuExtenderDelegates = FJJYYContentBrowserExtension_Impl::GetExtenderDelegates();
	CBMenuExtenderDelegates.RemoveAll([](const FContentBrowserMenuExtender_SelectedAssets& Delegate){ return Delegate.GetHandle() == ContentBrowserExtenderDelegateHandle; });
}

#undef LOCTEXT_NAMESPACE