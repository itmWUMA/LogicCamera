// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "CameraAction/LogicCameraActionManager.h"

#include "CameraAction/CameraActionBase.h"
#include "Core/LogicCameraDefines.h"
#include "Core/LogicDataConfig.h"
#include "Core/LogicPlayerCameraManager.h"

uint32 ULogicCameraActionManager::CameraActionDynamicPriority = 0;

ULogicCameraActionManager* ULogicCameraActionManager::Get(const UObject* WorldContext)
{
	UWorld* World = IsValid(WorldContext) ? WorldContext->GetWorld() : nullptr;
	return IsValid(World) ? World->GetSubsystem<ULogicCameraActionManager>() : nullptr;
}

void ULogicCameraActionManager::OnInit(ALogicPlayerCameraManager* LogicPlayerCameraManager)
{
	CamMgrCache = MakeWeakObjectPtr<ALogicPlayerCameraManager>(LogicPlayerCameraManager);
	CameraTrackList = NewObject<UCameraTrackList>(this);
}

void ULogicCameraActionManager::OnReset()
{
	CameraActionList.Reset();
	CamMgrCache.Reset();
	CameraActionDynamicPriority = 0;
	CameraTrackList = nullptr;
}

void ULogicCameraActionManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdatePendingRemoveCameraAction();
}

TStatId ULogicCameraActionManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(ULogicCameraActionManager, STATGROUP_Tickables);
}

FGuid ULogicCameraActionManager::AddCameraAction(UCameraActionBase* InCameraAction,
                                                 const FCameraActionBindData& BindingInfo)
{
	if (!IsValid(InCameraAction))
		return FGuid();

	TSharedPtr<FCameraActionInstance> Instance = GenerateCameraActionInstance(InCameraAction, BindingInfo);
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
	const FCameraActionBindData& BindingInfo)
{
	FGuid ID = FindCameraAction(InCameraAction);
	if (ID.IsValid())
		return ID;
	return AddCameraAction(InCameraAction, BindingInfo);
}

TSharedPtr<FCameraActionInstance> ULogicCameraActionManager::GenerateCameraActionInstance(
	UCameraActionBase* Source, const FCameraActionBindData& BindingInfo) const
{
	uint32 MapPriority = static_cast<uint32>(GetCameraActionMapPriority(BindingInfo));
	uint32 DynamicPriority = GetAndUpdateDynamicPriority();
	return MakeShared<FCameraActionInstance>(Source, BindingInfo, FGuid::NewGuid(), MapPriority, DynamicPriority);
}

int32 ULogicCameraActionManager::GetCameraActionMapPriority(const FCameraActionBindData& BindingInfo) const
{
	if (CamMgrCache.IsValid() && CamMgrCache->LogicCameraSettings)
	{
		if (const int32* PriorityPtr = CamMgrCache->LogicCameraSettings->CameraActionPriorityMap.Find(BindingInfo.PriorityName))
			return *PriorityPtr;
		
		UE_LOG(LogLC, Warning, TEXT("Invalid PriorityName[%s]"), *BindingInfo.PriorityName.ToString());
		return CamMgrCache->LogicCameraSettings->CameraActionPriorityMap[TEXT("Default")];
	}

	checkf(false, TEXT("PlayerCameraManager or LogicCameraSettings is invalid!"));
	return 0;
}

uint32 ULogicCameraActionManager::GetAndUpdateDynamicPriority()
{
	if (CameraActionDynamicPriority == MAX_uint32)
	{
		UE_LOG(LogLC, Warning, TEXT("CameraActionDynamicPriority is max value. Create too much camera action!"));
		return CameraActionDynamicPriority;
	}
	return CameraActionDynamicPriority++;
}

void ULogicCameraActionManager::UpdatePendingRemoveCameraAction()
{
	if (CameraActionList.IsEmpty())
		return;

	uint32 LIndex = 0, RIndex = CameraActionList.Num() - 1;

	// 遍历，将待移除的CA交换至队尾
	while (LIndex < RIndex)
	{
		while (LIndex < RIndex && CameraActionList[LIndex]->CurrentState != ECameraActionState::Finished)
			++LIndex;
		while (LIndex < RIndex && CameraActionList[RIndex]->CurrentState == ECameraActionState::Finished)
			--RIndex;
		CameraActionList.Swap(LIndex, RIndex);
	}

	// 拿到所有待移除的CA，并清空容器中待移除的CA
	TArray<TSharedPtr<FCameraActionInstance>> PendingToRemoveCameraActions;
	uint32 Length = CameraActionList.Num();
	for (LIndex = 0, RIndex = Length - 1; LIndex < Length - 1 && CameraActionList[Length - LIndex - 1]->CurrentState == ECameraActionState::Finished; ++LIndex)
		PendingToRemoveCameraActions.Emplace(CameraActionList[Length - LIndex - 1]);
	CameraActionList.RemoveAt(Length - LIndex, LIndex);

	// 处理待移除的CA
	for (TSharedPtr<FCameraActionInstance> PendingToRemoveCA : PendingToRemoveCameraActions)
		FinishCameraActionInternal(PendingToRemoveCA);
}

void ULogicCameraActionManager::FinishCameraActionInternal(TSharedPtr<FCameraActionInstance> InCameraActionInstance)
{
	if (InCameraActionInstance->CurrentState != ECameraActionState::Finished)
		return;

	InCameraActionInstance->BindingInfo.OnFinished.ExecuteIfBound();
	if (InCameraActionInstance->CameraActionCache.IsValid())
		InCameraActionInstance->CameraActionCache->Exit(CamMgrCache.Get());
	CameraTrackList->StopTracks(InCameraActionInstance->CameraActionCache.Get(), InCameraActionInstance->ActiveTracks);
	InCameraActionInstance->UnbindAllDelegates();
}
