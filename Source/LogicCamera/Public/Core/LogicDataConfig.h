// Copyright ITMWUMA Victor, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LogicDataConfig.generated.h"

/**
 * 
 */
UCLASS()
class LOGICCAMERA_API ULogicDataConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Camera Action", meta = (RowType = "/Script/LogicCamera.CameraActionInterruptPolicy"))
	TObjectPtr<UDataTable> CameraActionInterruptPolicies;
};
