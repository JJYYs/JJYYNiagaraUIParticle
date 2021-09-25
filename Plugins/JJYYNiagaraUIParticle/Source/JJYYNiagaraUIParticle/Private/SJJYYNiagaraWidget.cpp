// Copyright 2021 - Michal Smoleň

#include "SJJYYNiagaraWidget.h"
#include "SlateMaterialBrush.h"
#include "JJYYNiagaraComponent.h"


void SJJYYNiagaraWidget::Construct(const FArguments& Args)
{
}

SJJYYNiagaraWidget::~SJJYYNiagaraWidget()
{
    RenderData.Empty();
    CheckForInvalidBrushes();
}

int32 SJJYYNiagaraWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    if (!NiagaraComponent.IsValid())
        return LayerId;
    float A, B, C, D;
    AllottedGeometry.GetAccumulatedRenderTransform().GetMatrix().GetMatrix(A, B, C, D);
    FVector2D T = AllottedGeometry.GetAbsolutePositionAtCoordinates(FVector2D(0.5f, 0.5f)) / AllottedGeometry.Scale;
    FMatrix M3 = FMatrix(
        FPlane(A, 0.f, -B, 0.f),
        FPlane(0.f, 1.f, 0.f, 0.f),
        FPlane(-C, 0.f, D, 0.f),
        FPlane(T.X, 0.f, -T.Y, 1.f)
    );
    FTransform ComponentTransform(M3);
    NiagaraComponent->SetComponentTransform(ComponentTransform);
    NiagaraComponent->FillRenderData(const_cast<SJJYYNiagaraWidget*>(this), AllottedGeometry.GetAccumulatedLayoutTransform().GetScale(),ComponentTransform);

    return SMeshWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

}

void SJJYYNiagaraWidget::AddRenderData(FSlateVertex** OutVertexData, SlateIndex** OutIndexData, UMaterialInterface* Material, int32 NumVertexData, int32 NumIndexData)
{
    if (NumVertexData < 1 || NumIndexData < 1)
        return;

    FRenderData& NewRenderData = RenderData[RenderData.Add(FRenderData())];

    NewRenderData.VertexData.AddUninitialized(NumVertexData);
    *OutVertexData = &NewRenderData.VertexData[0];

    NewRenderData.IndexData.AddUninitialized(NumIndexData);
    *OutIndexData = &NewRenderData.IndexData[0];

    if (Material)
    {
        NewRenderData.Brush = CreateSlateMaterialBrush(Material);
        NewRenderData.RenderingResourceHandle = FSlateApplication::Get().GetRenderer()->GetResourceHandle(*NewRenderData.Brush);
    }
    AddRenderRun(RenderData.Num() - 1, 0, 1);
    FSlateInstanceBufferData InstanceBuffer;
    InstanceBuffer.Add(FVector4());
    UpdatePerInstanceBuffer(RenderData.Num() - 1, InstanceBuffer);
}

int32 SJJYYNiagaraWidget::AddRenderDataWithInstance(FSlateVertex** OutVertexData, SlateIndex** OutIndexData, UMaterialInterface* Material, int32 NumVertexData, int32 NumIndexData)
{
    if (NumVertexData < 1 || NumIndexData < 1)
        return -1;

    int32 RenderDataIndex = RenderData.Add(FRenderData());
    FRenderData& NewRenderData = RenderData[RenderDataIndex];

    NewRenderData.VertexData.AddUninitialized(NumVertexData);
    *OutVertexData = &NewRenderData.VertexData[0];

    NewRenderData.IndexData.AddUninitialized(NumIndexData);
    *OutIndexData = &NewRenderData.IndexData[0];

    if (Material)
    {
        NewRenderData.Brush = CreateSlateMaterialBrush(Material);
        NewRenderData.RenderingResourceHandle = FSlateApplication::Get().GetRenderer()->GetResourceHandle(*NewRenderData.Brush);
    }
    return RenderDataIndex;
}

TMap<UMaterialInterface*, TSharedPtr<FSlateMaterialBrush>> SJJYYNiagaraWidget::MaterialBrushMap;
TSharedPtr<FSlateMaterialBrush> SJJYYNiagaraWidget::CreateSlateMaterialBrush(UMaterialInterface* Material)
{
    UMaterialInterface* MaterialToUse = Material;
    if (MaterialBrushMap.Contains(MaterialToUse))
     {
         const auto MapElement = MaterialBrushMap.Find(MaterialToUse);

         if (MapElement->IsValid() && MapElement->Get()->GetResourceObject()->IsValidLowLevel() && MapElement->Get()->GetResourceObject()->IsA<UMaterialInterface>())
         {
             return *MapElement;
         }
     }

    const auto MaterialInstanceDynamic = UMaterialInstanceDynamic::Create(MaterialToUse, GetTransientPackage());
    TSharedPtr<FSlateMaterialBrush> NewElement = MakeShareable(new FSlateMaterialBrush(*MaterialInstanceDynamic, FVector2D(1.f, 1.f)));

    MaterialBrushMap.Add(MaterialToUse, NewElement);
    return NewElement;
}

void SJJYYNiagaraWidget::CheckForInvalidBrushes()
{
    TArray<UMaterialInterface*> RemoveMaterials;
    for (const auto& Brush : MaterialBrushMap)
    {
        if (Brush.Value.GetSharedReferenceCount() <= 1)
        {
            Brush.Value->GetResourceObject()->RemoveFromRoot();
            RemoveMaterials.Add(Brush.Key);
        }
    }

    for (const auto MaterialToRemove : RemoveMaterials)
    {
        MaterialBrushMap.Remove(MaterialToRemove);
    }
}

void SJJYYNiagaraWidget::ClearRenderData()
{
    RenderData.Empty();
}

void SJJYYNiagaraWidget::SetNiagaraComponent(TWeakObjectPtr<UJJYYNiagaraComponent> InNiagaraComponent)
{
    NiagaraComponent = InNiagaraComponent;
}
