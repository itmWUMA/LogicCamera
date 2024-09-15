// Copyright ITMWUMA Victor, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CameraTrack.h"
#include "CameraAction/CameraActionBase.h"
#include "Core/LogicCameraDefines.h"
#include "CameraTrackList.generated.h"

class ALogicPlayerCameraManager;
class UCameraActionBase;

/**
 * 
 */
UCLASS()
class LOGICCAMERA_API UCameraTrackList : public UObject
{
	GENERATED_BODY()

public:
	void InitTracks(TWeakObjectPtr<ALogicPlayerCameraManager> CamMgr);
	void ResetTracks();
	
	bool GetCurrentTrackValues(FCameraTrackValueCollection& OutParams) const;
	// 从相机参数中获取当前的轨道值
	bool CollectCurrentTrackValues(const ALogicPlayerCameraManager* CamMgr, FCameraTrackValueCollection& OutParams) const;
	
	void ActiveTracks(const UCameraActionBase* InCameraAction, uint16& ActiveTracks, const FCameraTrackValueCollection& TrackValues, uint64 Priority);
	void UpdateTracks(const UCameraActionBase* InCameraAction, uint16& ActiveTracks, const FCameraTrackValueCollection& TrackValues, uint64 Priority);
	void StopTracks(UCameraActionBase* InCameraAction, uint16 ActiveTracks);
	uint16 CheckTracksOccupy(uint16 ActiveTracks, uint64 Priority) const;
	bool CheckTracksAllArrived(uint16 ActiveTracks) const;

private:
	void BoundCameraActionParamsToTrack(FCameraTrack& Track, const UCameraActionBase* CameraAction);

private:
	TArray<FCameraTrack, TInlineAllocator<LC_CAMERA_TRACK_COUNT>> Tracks;
	TWeakObjectPtr<ALogicPlayerCameraManager> CamMgrCache = nullptr;
};
