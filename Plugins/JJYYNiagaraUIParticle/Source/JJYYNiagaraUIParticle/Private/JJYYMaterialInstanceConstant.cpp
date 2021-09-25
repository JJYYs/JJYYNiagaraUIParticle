// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "JJYYMaterialInstanceConstant.h"

UJJYYMaterialInstanceConstant::UJJYYMaterialInstanceConstant(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

bool UJJYYMaterialInstanceConstant::CheckMaterialUsage_Concurrent(const EMaterialUsage Usage) const
{
    UMaterial const* Material = GetMaterial_Concurrent();
    if (Material)
    {
        bool bUsageSetSuccessfully = false;
        if (JJYYNeedsSetMaterialUsage_Concurrent(bUsageSetSuccessfully, Usage))
        {
            if (IsInGameThread())
            {
                bUsageSetSuccessfully = const_cast<UJJYYMaterialInstanceConstant*>(this)->CheckMaterialUsage(Usage);
            }
            else
            {
                struct FCallSMU
                {
                    UMaterialInstance* Material;
                    EMaterialUsage Usage;

                    FCallSMU(UMaterialInstance* InMaterial, EMaterialUsage InUsage)
                        : Material(InMaterial)
                        , Usage(InUsage)
                    {
                    }

                    void Task()
                    {
                        Material->CheckMaterialUsage(Usage);
                    }
                };

                TSharedRef<FCallSMU, ESPMode::ThreadSafe> CallSMU = MakeShareable(new FCallSMU(const_cast<UJJYYMaterialInstanceConstant*>(this), Usage));
                bUsageSetSuccessfully = false;

                DECLARE_CYCLE_STAT(TEXT("FSimpleDelegateGraphTask.CheckMaterialUsage"),
                STAT_FSimpleDelegateGraphTask_CheckMaterialUsage,
                    STATGROUP_TaskGraphTasks);

                FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
                    FSimpleDelegateGraphTask::FDelegate::CreateThreadSafeSP(CallSMU, &FCallSMU::Task),
                    GET_STATID(STAT_FSimpleDelegateGraphTask_CheckMaterialUsage), NULL, ENamedThreads::GameThread_Local
                );
            }
        }
        return bUsageSetSuccessfully;
    }
    else
    {
        return false;
    }

    

}

bool UJJYYMaterialInstanceConstant::JJYYNeedsSetMaterialUsage_Concurrent(bool& bOutHasUsage, const EMaterialUsage Usage) const
{
    bOutHasUsage = true;
    UMaterial* Material = const_cast<UMaterial*>(GetMaterial());
    // Check that the material has been flagged for use with the given usage flag.
    if (!Material->GetUsageByFlag(Usage) && !Material->bUsedAsSpecialEngineMaterial)
    {
        uint32 UsageFlagBit = (1 << (uint32)Usage);
        if ((Material->UsageFlagWarnings & UsageFlagBit) == 0)
        {
            // This will be overwritten later by SetMaterialUsage, since we are saying that it needs to be called with the return value
            bOutHasUsage = false;
            return true;
        }
        else
        {
            // We have already warned about this, so we aren't going to warn or compile or set anything this time
            bOutHasUsage = false;
            return false;
        }
    }
    return false;
}

bool UJJYYMaterialInstanceConstant::CheckMaterialUsage(const EMaterialUsage Usage)
{
    check(IsInGameThread());
    UMaterial* Material = GetMaterial();
    if (Material)
    {

#if WITH_EDITOR
        bool bUsageSetSuccessfully = false;
        if (!Material->bUsedWithNiagaraSprites)
        {
            Material->Modify();
            Material->bUsedWithNiagaraSprites = 1;
            Material->ForceRecompileForRendering();
            Material->MarkPackageDirty();
            bUsageSetSuccessfully = true;
        }
        else if (!Material->bUsedWithNiagaraRibbons)
        {
            Material->Modify();
            Material->bUsedWithNiagaraRibbons = 1;
            Material->ForceRecompileForRendering();
            Material->MarkPackageDirty();
            bUsageSetSuccessfully = true;
        }
        else if (!Material->bUsedWithNiagaraMeshParticles)
        {
            Material->Modify();
            Material->bUsedWithNiagaraMeshParticles = 1;
            Material->ForceRecompileForRendering();
            Material->MarkPackageDirty();
            bUsageSetSuccessfully = true;
        }

        return bUsageSetSuccessfully;
#else
        return true;
#endif
    }
    else
    {
        return false;
    }
}

void UJJYYMaterialInstanceConstant::InitResources()
{
    Super::InitResources();
}

