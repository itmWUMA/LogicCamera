// Copyright ITMWUMA Victor, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECameraTrackType : uint8
{
	Roll = 0,
	Pitch,
	Yaw,
	ArmLength,
	SocketOffset_X,
	SocketOffset_Y,
	SocketOffset_Z,
	TargetOffset_X,
	TargetOffset_Y,
	TargetOffset_Z,
	FOV,
	Max,
};

#define LC_INVALID_VALUE BIG_NUMBER
#define LC_CAMERA_TRACK_COUNT static_cast<uint8>(ECameraTrackType::Max)
#define LC_DEFAUTL_CAMERA_ACTION_PRIORITY 100

LOGICCAMERA_API DECLARE_LOG_CATEGORY_EXTERN(LogLC, Log, All);