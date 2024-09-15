// Copyright ITMWUMA Victor, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CameraAction/CameraActionBase.h"
#include "CameraAction/CameraActionInstance.h"
#include "Core/LogicCameraDefines.h"

/**
 * 相机轨道，用于存储和管理一个float类型的相机参数
 * 如Pitch、Yaw、Roll、ArmLength等
 */
struct LOGICCAMERA_API FCameraTrack
{
	friend class UCameraTrackList;

	FCameraTrack() = default;
	FCameraTrack(uint8 InTrackID) : TrackID(InTrackID)
	{
		CurPriority.Priority = 0;
	}

	// 设置目标值
	void SetTargetValue(float InTargetValue);
	// 更新当前值
	void UpdateCurValue(float InValue);

	void StopTrack();
	bool IsArriveTarget() const;
	
	uint8 TrackID = 0;
	CameraActionPriority CurPriority;

	// 轨道是否激活标识（激活后进行轨道值更新计算）
	bool bEffective = false;
	
	// 当前值，会向目标值进行插值
	float CurValue = LC_INVALID_VALUE;
	// 目标值
	float TargetValue = LC_INVALID_VALUE;

	TWeakObjectPtr<const UCameraActionBase> CameraActionCache = nullptr;
	FCameraActionSpeedData SpeedData;
	FCameraActionSpeedData DownSpeedData;
	float Tolerance = 1.f;
};
