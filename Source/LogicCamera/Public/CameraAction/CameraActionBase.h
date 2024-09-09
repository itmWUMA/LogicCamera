// Copyright ITMWUMA Victor, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Track/CameraTrackList.h"
#include "UObject/NoExportTypes.h"
#include "CameraActionBase.generated.h"

class ALogicPlayerCameraManager;
/**
 * 相机行为
 * 
 * 用于描述一段镜头效果（相机的行为），如旋转、拉近拉远、FOV调整等
 * 相机行为会控制8个轨道，根据相关接口（如Enter、Tick等）应用相应的轨道值，以完成相机行为的控制
 * 
 * 是整个相机框架中的最小单位
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew)
class LOGICCAMERA_API UCameraActionBase : public UObject
{
	GENERATED_BODY()

public:
	// 执行CA前，用于初始化参数和状态设置
	virtual void Prepare(ALogicPlayerCameraManager* CamMgr);
	
	// 开始执行CA
	virtual void Enter(ALogicPlayerCameraManager* CamMgr, const FCameraTrackValueCollection& CurTrackValues, OUT FCameraTrackValueCollection& OutTrackValues);
	
	// 执行CA的过程中
	virtual void Update(ALogicPlayerCameraManager* CamMgr, float DeltaTime, const FCameraTrackValueCollection& CurTrackValues, OUT FCameraTrackValueCollection& OutTrackValues);
	
	// 打断CA，需要开启持续相机行为属性
	virtual void Interrupt(ALogicPlayerCameraManager* CamMgr);
	
	// 恢复CA，需要开启持续相机行为属性
	virtual void Resume(ALogicPlayerCameraManager* CamMgr);
	
	// 退出CA
	virtual void Exit(ALogicPlayerCameraManager* CamMgr);
};
