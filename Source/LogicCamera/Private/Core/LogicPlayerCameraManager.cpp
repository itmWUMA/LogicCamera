// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "Core/LogicPlayerCameraManager.h"

#include "CameraAction/LogicCameraActionManager.h"
#include "Core/LogicDataConfig.h"
#include "Core/LogicMainCamera.h"

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

void ALogicPlayerCameraManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!LogicCameraSettings)
		return;
	
	ALogicMainCamera* MainCamera = GetWorld()->SpawnActor<ALogicMainCamera>(LogicCameraSettings->MainCameraClass);
	MainCameraCache = MainCamera;
}

void ALogicPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();

	if (!MainCameraCache.IsValid())
		return;

	SetViewTarget(MainCameraCache.Get());

	if (ULogicCameraActionManager* CameraActionMgr = ULogicCameraActionManager::Get(this))
	{
		CameraActionMgr->OnInit(this);
	}
}
