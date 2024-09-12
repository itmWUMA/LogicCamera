// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "CameraAction/SimpleCameraAction.h"

void USimpleCameraAction::OnEnter_Implementation(ALogicPlayerCameraManager* CamMgr,
	const FCameraTrackValueCollection& CurTrackValues, FCameraTrackValueCollection& OutTrackValues)
{
	OutTrackValues = Params;
}

void USimpleCameraAction::OnUpdate_Implementation(ALogicPlayerCameraManager* CamMgr, float DeltaTime,
	const FCameraTrackValueCollection& CurTrackValues, FCameraTrackValueCollection& OutTrackValues)
{
	if (bEnableTick)
	{
		OutTrackValues = Params;
	}
}
