// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "Core/LogicDataConfig.h"

#include "Core/LogicCameraDefines.h"

ULogicDataConfig::ULogicDataConfig()
{
	CameraActionPriorityMap.Add(TEXT("Default"), LC_DEFAULT_CAMERA_ACTION_PRIORITY);
}