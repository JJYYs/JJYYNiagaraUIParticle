// Copyright 2021 - Michal Smoleň

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "JJYYNiagaraWidget.generated.h"

class SJJYYNiagaraWidget;

USTRUCT()
struct FSlateMeshData
{
    GENERATED_BODY()

        UPROPERTY()
        FName MeshPackageName;
    UPROPERTY()
        TArray<FVector2D> Vertex;
    UPROPERTY()
        TArray<FColor> VertexColor;
    UPROPERTY()
        TArray<FVector2D> UV;
    UPROPERTY()
        TArray<uint32> Index;
};

/**
* 
 */
UCLASS()
class JJYYNIAGARAUIPARTICLE_API UJJYYNiagaraWidget : public UWidget
{
	GENERATED_BODY()
		
	virtual TSharedRef<SWidget> RebuildWidget() override;
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

    void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif

public:
    UFUNCTION(BlueprintCallable, Category = "JJYY Particle")
        void UpdateNiagaraSystem(class UNiagaraSystem* NewNiagaraSystem);
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JJYY Particle", BlueprintSetter = UpdateNiagaraSystem)
        class UNiagaraSystem* NiagaraSystem;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JJYY Particle")
        bool AutoActivate = true;

    UPROPERTY()
        mutable TArray<FSlateMeshData> MeshData;
private:
    void InitNiagaraComponent();
       
private:
	TSharedPtr<SJJYYNiagaraWidget> JJYYNiagaraSlateWidget;
    UPROPERTY()
    class UJJYYNiagaraComponent* NiagaraComponent;
};
