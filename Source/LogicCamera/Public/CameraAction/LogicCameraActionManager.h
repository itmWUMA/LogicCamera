// Copyright ITMWUMA Victor, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "LogicCameraActionManager.generated.h"

/**
 * 相机行为的管理器
 *
 * 1. 存储相机轨道，并根据轨道值应用到主相机中，使得轨道值时机影响到相机表现
 * 2. 管理相机行为，处理相机行为间的关系，如结束、打断、恢复等
 */
UCLASS()
class LOGICCAMERA_API ULogicCameraActionManager : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
};
