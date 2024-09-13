// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "CameraAction/LogicCameraActionManager.h"

#include "CameraAction/CameraActionBase.h"
#include "Core/LogicCameraDefines.h"
#include "Core/LogicDataConfig.h"
#include "Core/LogicPlayerCameraManager.h"

uint64 ULogicCameraActionManager::CameraActionDynamicPriority = 0;

void ULogicCameraActionManager::OnInit(ALogicPlayerCameraManager* LogicPlayerCameraManager)
{
	CamMgrCache = MakeWeakObjectPtr<ALogicPlayerCameraManager>(LogicPlayerCameraManager);
}

void ULogicCameraActionManager::OnReset()
{
	CamMgrCache.Reset();
}

FGuid ULogicCameraActionManager::AddCameraAction(UCameraActionBase* InCameraAction,
	const FCameraActionInstanceGenerateInfo& GenerateInfo)
{
	if (!IsValid(InCameraAction))
		return FGuid();

	TSharedPtr<FCameraActionInstance> Instance = GenerateCameraActionInstance(InCameraAction, GenerateInfo);
	CameraActionList.Push(Instance);
	
	return Instance->ID;
}

FGuid ULogicCameraActionManager::FindCameraAction(UCameraActionBase* InCameraAction) const
{
	if (!IsValid(InCameraAction))
		return FGuid();
	
	const TSharedPtr<FCameraActionInstance>* InstancePtr = CameraActionList.FindByPredicate([InCameraAction](const TSharedPtr<FCameraActionInstance>& InInstance) -> bool
	{
		return InInstance->CameraActionCache == InCameraAction;
	});

	return InstancePtr ? (*InstancePtr)->ID : FGuid();
}

FGuid ULogicCameraActionManager::FindOrAddCameraAction(UCameraActionBase* InCameraAction,
	const FCameraActionInstanceGenerateInfo& GenerateInfo)
{
	FGuid ID = FindCameraAction(InCameraAction);
	if (ID.IsValid())
		return ID;
	return AddCameraAction(InCameraAction, GenerateInfo);
}

TSharedPtr<FCameraActionInstance> ULogicCameraActionManager::GenerateCameraActionInstance(
	const UCameraActionBase* Source, const FCameraActionInstanceGenerateInfo& GenerateInfo) const
{
	uint32 MapPriority = static_cast<uint32>(GetCameraActionMapPriority(GenerateInfo));
	uint32 DynamicPriority = GetAndUpdateDynamicPriority();
	return MakeShared<FCameraActionInstance>(Source, GenerateInfo, FGuid::NewGuid(), MapPriority, DynamicPriority);
}

int32 ULogicCameraActionManager::GetCameraActionMapPriority(const FCameraActionInstanceGenerateInfo& GenerateInfo) const
{
	if (CamMgrCache.IsValid() && CamMgrCache->LogicCameraSettings)
	{
		if (const int32* PriorityPtr = CamMgrCache->LogicCameraSettings->CameraActionPriorityMap.Find(GenerateInfo.PriorityName))
			return *PriorityPtr;
		
		UE_LOG(LogLC, Warning, TEXT("Invalid PriorityName[%s]"), *GenerateInfo.PriorityName.ToString());
		return CamMgrCache->LogicCameraSettings->CameraActionPriorityMap[TEXT("Default")];
	}

	checkf(false, TEXT("PlayerCameraManager or LogicCameraSettings is invalid!"));
	return 0;
}

uint64 ULogicCameraActionManager::GetAndUpdateDynamicPriority()
{
	if (CameraActionDynamicPriority == MAX_uint32)
	{
		UE_LOG(LogLC, Warning, TEXT("CameraActionDynamicPriority is max value. Create too much camera action!"));
		return CameraActionDynamicPriority;
	}
	return CameraActionDynamicPriority++;
}
