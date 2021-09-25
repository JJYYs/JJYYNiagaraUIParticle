// Copyright 2021 - Michal Smoleň

#pragma once

#include "Slate/SMeshWidget.h"

class UMaterialInterface;
class UJJYYNiagaraComponent;
struct FSlateMaterialBrush;
/**
 * 
 */
class JJYYNIAGARAUIPARTICLE_API SJJYYNiagaraWidget : public SMeshWidget
{	
public:
	SLATE_BEGIN_ARGS(SJJYYNiagaraWidget)
	{		
	}
	SLATE_END_ARGS()

	void Construct(const FArguments& Args);
	
	~SJJYYNiagaraWidget();

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

    void AddRenderData(FSlateVertex** OutVertexData, SlateIndex** OutIndexData, UMaterialInterface* Material, int32 NumVertexData, int32 NumIndexData);
	int32 AddRenderDataWithInstance(FSlateVertex** OutVertexData, SlateIndex** OutIndexData, UMaterialInterface* Material, int32 NumVertexData, int32 NumIndexData);
    TSharedPtr<FSlateMaterialBrush> CreateSlateMaterialBrush(UMaterialInterface* Material);
	void CheckForInvalidBrushes();
	void ClearRenderData();

    void SetNiagaraComponent(TWeakObjectPtr<UJJYYNiagaraComponent> InNiagaraComponent);

private:
    TWeakObjectPtr<UJJYYNiagaraComponent> NiagaraComponent;
    static TMap<UMaterialInterface*, TSharedPtr<FSlateMaterialBrush>> MaterialBrushMap;
	
};
