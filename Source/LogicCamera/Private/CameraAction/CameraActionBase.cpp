// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "CameraAction/CameraActionBase.h"

void UCameraActionBase::Prepare(ALogicPlayerCameraManager* CamMgr)
{
	BP_OnPrepare(CamMgr);
}

void UCameraActionBase::Enter(ALogicPlayerCameraManager* CamMgr, const FCameraTrackValueCollection& CurTrackValues,
	FCameraTrackValueCollection& OutTrackValues)
{
	BP_OnEnter(CamMgr, CurTrackValues, OutTrackValues);
}

void UCameraActionBase::Update(ALogicPlayerCameraManager* CamMgr, float DeltaTime,
	const FCameraTrackValueCollection& CurTrackValues, FCameraTrackValueCollection& OutTrackValues)
{
	BP_OnUpdate(CamMgr, DeltaTime, CurTrackValues, OutTrackValues);
}

void UCameraActionBase::Interrupt(ALogicPlayerCameraManager* CamMgr)
{
	BP_OnInterrupt(CamMgr);
}

void UCameraActionBase::Resume(ALogicPlayerCameraManager* CamMgr)
{
	BP_OnResume(CamMgr);
}

void UCameraActionBase::Exit(ALogicPlayerCameraManager* CamMgr)
{
	BP_OnExit(CamMgr);
}
