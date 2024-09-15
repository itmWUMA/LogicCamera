// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "Core/LogicPlayerCameraManager.h"

#include "CameraAction/LogicCameraActionManager.h"

void ALogicPlayerCameraManager::AssignViewTarget(AActor* NewTarget, FTViewTarget& VT,
	struct FViewTargetTransitionParams TransitionParams)
{
	Super::AssignViewTarget(NewTarget, VT, TransitionParams);

	if (ULogicCameraActionManager* CameraActionMgr = ULogicCameraActionManager::Get(this))
	{
		CameraActionMgr->OnReset();
	}
}

bool ALogicPlayerCameraManager::CollectCurrentTrackValues(FCameraTrackValueCollection& OutParams) const
{
	return true;
}

void ALogicPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();

	if (ULogicCameraActionManager* CameraActionMgr = ULogicCameraActionManager::Get(this))
	{
		CameraActionMgr->OnInit(this);
	}
}
