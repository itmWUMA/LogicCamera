// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "Track/CameraTrack.h"

#include "Utilities/LogicCameraMathUtilities.h"

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

void FCameraTrack::Update(float DeltaTime)
{
	if (!bEffective)
		return;

	bool bIsDown = CurValue > TargetValue;
	const FCameraActionSpeedData& FinalInterpSpeed = bIsDown ? DownSpeedData : SpeedData;
	float ResultCurValue = CurValue;

	if (FinalInterpSpeed.bUseConstInterp)
	{
		ResultCurValue = FMath::FInterpConstantTo(CurValue, TargetValue, DeltaTime, FinalInterpSpeed.InterpSpeed);
	}
	else
	{
		//TODO: 轨道趋近终点时，截断计算

		ResultCurValue = FLogicCameraMathUtilities::TrackInterpTo(
			CurValue,
			TargetValue,
			DeltaTime,
			FinalInterpSpeed.InterpSpeed,
			FinalInterpSpeed.Exponent);
	}

	CurValue = ResultCurValue;
}

FString FCameraTrack::GetDebugInfo() const
{
	auto ShowCameraDebugValue = [](float Value) -> float
	{
		return Value == LC_INVALID_VALUE ? -1.f : Value;
	};
	
	return FString::Printf(TEXT("[%s]\tCur:%.2f Tar:%.2f Speed:%.2f Exp:%.2f DownSpeed:%.2f DownExp:%.2f Tol:%.2f [%s]"),
		*UEnum::GetValueAsString(static_cast<ECameraTrackType>(TrackID)),
		ShowCameraDebugValue(CurValue),
		ShowCameraDebugValue(TargetValue),
		SpeedData.InterpSpeed,
		SpeedData.Exponent,
		DownSpeedData.InterpSpeed,
		DownSpeedData.Exponent,
		Tolerance,
		*(!CameraActionCache.IsValid() ? TEXT("None") : CameraActionCache->GetName()));
}
