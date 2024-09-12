// Copyright ITMWUMA Victor, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CameraAction/CameraActionBase.h"
#include "SimpleCameraAction.generated.h"

/**
 * 简单相机行为，可以直接给定一组相机轨道值并应用
 */
UCLASS()
class LOGICCAMERA_API USimpleCameraAction : public UCameraActionBase
{
	GENERATED_BODY()

protected:
	virtual void OnEnter_Implementation(ALogicPlayerCameraManager* CamMgr, const FCameraTrackValueCollection& CurTrackValues, FCameraTrackValueCollection& OutTrackValues) override;
	virtual void OnUpdate_Implementation(ALogicPlayerCameraManager* CamMgr, float DeltaTime, const FCameraTrackValueCollection& CurTrackValues, FCameraTrackValueCollection& OutTrackValues) override;

private:
	UPROPERTY(EditAnywhere)
	FCameraTrackValueCollection Params;

	// 是否Tick时也覆盖轨道值，如果否则仅在Enter时设置轨道值
	UPROPERTY(EditAnywhere)
	bool bEnableTick = false;
};
