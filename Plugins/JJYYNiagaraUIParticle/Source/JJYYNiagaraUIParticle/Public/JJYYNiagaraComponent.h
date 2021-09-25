// Copyright 2021 - Michal Smoleň

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "JJYYNiagaraComponent.generated.h"

class SJJYYNiagaraWidget;

/**
 * 
 */
UCLASS()
class JJYYNIAGARAUIPARTICLE_API UJJYYNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
public:
	void SetComponentTransform(const FTransform& Transform);
	void FillRenderData(SJJYYNiagaraWidget* SlateWidet, const float SlateLayoutScale, const FTransform& ComponentTransform);
    void AddSpriteRendererData(SJJYYNiagaraWidget* SlateWidet, TSharedRef<const FNiagaraEmitterInstance, ESPMode::ThreadSafe> EmitterInst,
        class UNiagaraSpriteRendererProperties* SpriteRenderer, const float SlateLayoutScale, const FTransform& ComponentTransform);
    void AddRibbonRendererData(SJJYYNiagaraWidget* SlateWidet, TSharedRef<const FNiagaraEmitterInstance, ESPMode::ThreadSafe> EmitterInst,
        class UNiagaraRibbonRendererProperties* RibbonRenderer, const float SlateLayoutScale, const FTransform& ComponentTransform);
    void AddMeshRendererData(SJJYYNiagaraWidget* SlateWidet, TSharedRef<const FNiagaraEmitterInstance, ESPMode::ThreadSafe> EmitterInst,
        class UNiagaraMeshRendererProperties* MeshRenderer, const float SlateLayoutScale, const FTransform& ComponentTransform);
};
