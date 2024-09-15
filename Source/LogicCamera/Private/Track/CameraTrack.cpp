// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "Track/CameraTrack.h"

void FCameraTrack::SetTargetValue(float InTargetValue)
{
	this->TargetValue = InTargetValue;
}

void FCameraTrack::UpdateCurValue(float InValue)
{
	CurValue = InValue;
	
	// 旋转类轨道值需要做欧拉角限制
	if (TrackID <= static_cast<uint8>(ECameraTrackType::Yaw))
	{
		CurValue = FRotator::NormalizeAxis(CurValue);
		if (TargetValue != LC_INVALID_VALUE)
		{
			// 取角度的最短路径
			FMath::WindRelativeAnglesDegrees(TargetValue, CurValue);
		}
	}
}

void FCameraTrack::StopTrack()
{
	SetTargetValue(LC_INVALID_VALUE);
	SpeedData = DownSpeedData = FCameraActionSpeedData();
	CurPriority.Priority = 0;
	bEffective = false;
	CameraActionCache.Reset();
}

bool FCameraTrack::IsArriveTarget() const
{
	return TargetValue == LC_INVALID_VALUE ? true : FMath::IsNearlyEqual(TargetValue, CurValue, Tolerance);
}
