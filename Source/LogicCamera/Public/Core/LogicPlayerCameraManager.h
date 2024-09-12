// Copyright ITMWUMA Victor, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "LogicPlayerCameraManager.generated.h"

class ULogicDataConfig;

/**
 * 
 */
UCLASS()
class LOGICCAMERA_API ALogicPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TObjectPtr<ULogicDataConfig> LogicCameraSettings;
};
