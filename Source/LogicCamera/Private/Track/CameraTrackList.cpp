// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "Track/CameraTrackList.h"

#include "Core/LogicCameraStatics.h"
#include "Core/LogicMainCamera.h"
#include "Core/LogicPlayerCameraManager.h"

void UCameraTrackList::InitTracks(TWeakObjectPtr<ALogicPlayerCameraManager> CamMgr)
{
	if (!Tracks.IsEmpty())
		Tracks.Reset();

	uint8 Index = 0;
	for (TFieldIterator<FFloatProperty> it(FCameraTrackValueCollection::StaticStruct()); it; ++it)
	{
		if (FFloatProperty* prop = *it)
		{
			Tracks.Push(FCameraTrack(Index));
			Index++;
		}
	}

	CamMgrCache = CamMgr;
	if (CamMgrCache.IsValid())
	{
		FCameraTrackValueCollection InitParams;
		if (CollectCurrentTrackValues(CamMgrCache.Get(), InitParams))
		{
			for (uint8 i = 0; i < Tracks.Num(); ++i)
			{
				Tracks[i].UpdateCurValue(InitParams[i]);
				Tracks[i].CurPriority.Priority = 0;
			}
		}
	}
}

void UCameraTrackList::ResetTracks()
{
	if (Tracks.IsEmpty())
		return;
	
	for (int8 Index = LC_CAMERA_TRACK_COUNT - 1; Index >= 0; --Index)
		Tracks[Index].StopTrack();
	
	CamMgrCache.Reset();
	Tracks.Reset();
}

bool UCameraTrackList::GetCurrentTrackValues(FCameraTrackValueCollection& OutParams) const
{
	for (uint8 Index = 0; Index < Tracks.Num(); ++Index)
		OutParams[Index] = Tracks[Index].CurValue;
	return true;
}

bool UCameraTrackList::CollectCurrentTrackValues(const ALogicPlayerCameraManager* CamMgr,
                                                 FCameraTrackValueCollection& OutParams) const
{
	return CamMgr ? CamMgr->CollectCurrentTrackValues(OutParams) : false;
}

void UCameraTrackList::ActiveTracks(const UCameraActionBase* InCameraAction, uint16& ActiveTracks,
                                    const FCameraTrackValueCollection& TrackValues, uint64 Priority)
{
	UpdateTracks(InCameraAction, ActiveTracks, TrackValues, Priority);
}

void UCameraTrackList::UpdateTracks(const UCameraActionBase* InCameraAction, uint16& ActiveTracks,
	const FCameraTrackValueCollection& TrackValues, uint64 Priority)
{
	uint16 TrackID = 1;
	for (int8 Index = LC_CAMERA_TRACK_COUNT - 1; Index >= 0; --Index)
	{
		if (ActiveTracks & TrackID)
		{
			if (Priority < Tracks[Index].CurPriority.Priority)
			{
				ActiveTracks &= ~TrackID;
				TrackID = TrackID << 1;
				continue;
			}
			
			Tracks[Index].CameraActionCache = MakeWeakObjectPtr<const UCameraActionBase>(InCameraAction);
			Tracks[Index].SetTargetValue(TrackValues[Index]);
			BoundCameraActionParamsToTrack(Tracks[Index], InCameraAction);
	
			Tracks[Index].CurPriority.Priority = Priority;
			Tracks[Index].bEffective = true;
		}
	
		TrackID = TrackID << 1;
	}
}

void UCameraTrackList::StopTracks(UCameraActionBase* InCameraAction, uint16 ActiveTracks)
{
	uint16 TrackID = 1;
	for (int8 Index = LC_CAMERA_TRACK_COUNT - 1; Index >= 0; --Index)
	{
		if (ActiveTracks & TrackID)
		{
			if (InCameraAction != Tracks[Index].CameraActionCache)
				continue;
			Tracks[Index].StopTrack();
		}
		TrackID = TrackID << 1;
	}
}

uint16 UCameraTrackList::CheckTracksOccupy(uint16 ActiveTracks, uint64 Priority) const
{
	uint16 OccupiedTracks = 0;
	uint16 TrackID = 1;
	
	for (int8 Index = LC_CAMERA_TRACK_COUNT - 1; Index >= 0; --Index)
	{
		if (ActiveTracks & TrackID)
		{
			if (Priority < Tracks[Index].CurPriority.Priority)
			{
				OccupiedTracks |= TrackID;
				TrackID = TrackID << 1;
				continue;
			}
		}
		TrackID = TrackID << 1;
	}

	return OccupiedTracks;
}

bool UCameraTrackList::CheckTracksAllArrived(uint16 ActiveTracks) const
{
	uint16 TrackID = 1;
	for (int8 Index = LC_CAMERA_TRACK_COUNT - 1; Index >= 0; --Index)
	{
		if (ActiveTracks & TrackID)
		{
			if (!Tracks[Index].IsArriveTarget())
				return false;
		}
		TrackID = TrackID << 1;
	}
	return true;
}

void UCameraTrackList::Update(float DeltaTime)
{
	// 更新轨道值计算
	for (FCameraTrack& Track : Tracks)
		Track.Update(DeltaTime);

	// 应用轨道值
	if (CamMgrCache.IsValid() && IsValid(CamMgrCache->GetMainCamera()))
	{
		for (FCameraTrack& Track : Tracks)
			CamMgrCache->GetMainCamera()->ApplyTrackValue(Track);

		//TODO 边界值限制
	}
}

void UCameraTrackList::ShowTracksDebug() const
{
	if (!GEngine)
		return;

	uint64 Channel = LC_CAMERA_TRACK_DEBUG_CHANNEL;
	for (const FCameraTrack& Track : Tracks)
	{
		GEngine->AddOnScreenDebugMessage(Channel++, 0.05f, FColor::Orange, Track.GetDebugInfo());
	}
}

void UCameraTrackList::BoundCameraActionParamsToTrack(FCameraTrack& Track, const UCameraActionBase* CameraAction)
{
	if (!IsValid(CameraAction))
		return;

	Track.SpeedData = CameraAction->InterpSpeedData;
	Track.DownSpeedData = CameraAction->bUseDownInterpSpeed ? CameraAction->DownInterpSpeedData : CameraAction->InterpSpeedData;
	Track.Tolerance = CameraAction->Tolerance;
}
