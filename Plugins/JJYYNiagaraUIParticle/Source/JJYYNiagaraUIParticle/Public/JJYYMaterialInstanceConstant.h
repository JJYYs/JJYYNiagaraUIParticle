// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Classes/Materials/MaterialInstanceConstant.h"
#include "JJYYMaterialInstanceConstant.generated.h"


/**
 * JJYY Material Instances may be used to UI and Niagara.
 */

UCLASS()
class JJYYNIAGARAUIPARTICLE_API  UJJYYMaterialInstanceConstant : public UMaterialInstanceConstant
{
	GENERATED_UCLASS_BODY()
	
public:
    bool CheckMaterialUsage_Concurrent(const EMaterialUsage Usage) const override;
	
	bool JJYYNeedsSetMaterialUsage_Concurrent(bool& bOutHasUsage, const EMaterialUsage Usage) const;

	bool CheckMaterialUsage(const EMaterialUsage Usage) override;

	void InitResources();

};
