// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "CameraAction/CameraActionBase.h"

void UCameraActionBase::Prepare(ALogicPlayerCameraManager* CamMgr)
{
	OnPrepare(CamMgr);
}

uint16 UCameraActionBase::Enter(ALogicPlayerCameraManager* CamMgr, const FCameraTrackValueCollection& CurTrackValues,
	FCameraTrackValueCollection& OutTrackValues)
{
	FCameraTrackValueCollection TempOutTrackValues;
	OnEnter(CamMgr, CurTrackValues, TempOutTrackValues);

	return GetTrackValue(TempOutTrackValues, OutTrackValues);
}

uint16 UCameraActionBase::Update(ALogicPlayerCameraManager* CamMgr, float DeltaTime,
	const FCameraTrackValueCollection& CurTrackValues, FCameraTrackValueCollection& OutTrackValues)
{
	FCameraTrackValueCollection TempOutTrackValues;
	OnUpdate(CamMgr, DeltaTime, CurTrackValues, TempOutTrackValues);

	return GetTrackValue(TempOutTrackValues, OutTrackValues);
}

void UCameraActionBase::Interrupt(ALogicPlayerCameraManager* CamMgr)
{
	OnInterrupt(CamMgr);
}

void UCameraActionBase::Resume(ALogicPlayerCameraManager* CamMgr)
{
	OnResume(CamMgr);
}

void UCameraActionBase::Exit(ALogicPlayerCameraManager* CamMgr)
{
	OnExit(CamMgr);
}

uint16 UCameraActionBase::GetTrackValue(const FCameraTrackValueCollection& TempOutTrackValues,
	FCameraTrackValueCollection& OutTrackValues)
{
	uint16 ActivatedTrackID = 0;
	uint16 CurTrackID = 1;
	
    for (int8 i = LC_CAMERA_TRACK_COUNT - 1; i >= 0; --i)
    {
    	float CurTrackValue = TempOutTrackValues[i];
    	// ClampValue
    	CurTrackValue = FMath::Min(CurTrackValue, LC_INVALID_VALUE);

    	if (CurTrackValue != LC_INVALID_VALUE)
    	{
    		OutTrackValues[i] = CurTrackValue;
    		ActivatedTrackID |= CurTrackID;
    	}

    	CurTrackID <<= 1;
    }

	return ActivatedTrackID;
}
