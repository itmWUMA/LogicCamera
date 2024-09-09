// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "CameraAction/CameraActionBase.h"

void UCameraActionBase::Prepare(ALogicPlayerCameraManager* CamMgr)
{
}

void UCameraActionBase::Enter(ALogicPlayerCameraManager* CamMgr, const FCameraTrackValueCollection& CurTrackValues,
	FCameraTrackValueCollection& OutTrackValues)
{
}

void UCameraActionBase::Update(ALogicPlayerCameraManager* CamMgr, float DeltaTime,
	const FCameraTrackValueCollection& CurTrackValues, FCameraTrackValueCollection& OutTrackValues)
{
}

void UCameraActionBase::Interrupt(ALogicPlayerCameraManager* CamMgr)
{
}

void UCameraActionBase::Resume(ALogicPlayerCameraManager* CamMgr)
{
}

void UCameraActionBase::Exit(ALogicPlayerCameraManager* CamMgr)
{
}
