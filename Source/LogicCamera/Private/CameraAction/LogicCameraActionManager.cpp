// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "CameraAction/LogicCameraActionManager.h"

#include "CameraAction/CameraActionBase.h"
#include "CameraAction/InterruptPolicy/CameraActionInterruptPolicy.h"
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

	if (!CamMgrCache.IsValid())
		return;

	UpdatePendingRemoveCameraAction();
	UpdateCameraAction(DeltaTime);
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

void ULogicCameraActionManager::UpdateCameraAction(float DeltaTime)
{
	SortCameraActionList();

	for (const TSharedPtr<FCameraActionInstance>& CurFrameInst : CurFrameCameraAnimInstance)
	{
		if (!CurFrameInst->CameraActionCache.IsValid())
		{
			CurFrameInst->CurrentState = ECameraActionState::Finished;
			continue;
		}

		UCameraActionBase* CurFrameCameraAction = CurFrameInst->CameraActionCache.Get();
		
		if (CurFrameInst->CurrentState == ECameraActionState::Awake)
		{
			CurFrameCameraAction->Prepare(CamMgrCache.Get());
			CurFrameInst->BindParams();
			EnterCameraActionInternal(CurFrameInst);
		}

		if (CurFrameInst->CurrentState == ECameraActionState::Executing)
		{
			UpdateCameraActionInternal(CurFrameInst, DeltaTime);
		}
	}

	for (const TSharedPtr<FCameraActionInstance>& PreFrameInst : PreFrameCameraAnimInstance)
	{
		if (PreFrameInst->CurrentState == ECameraActionState::Executing && !CurFrameCameraAnimInstance.Contains(PreFrameInst))
		{
			if (PreFrameInst->CurrentState == ECameraActionState::Finished)
				continue;
			InterruptCameraActionInternal(PreFrameInst);
		}
	}

	::Swap(PreFrameCameraAnimInstance, CurFrameCameraAnimInstance);

	for (const TSharedPtr<FCameraActionInstance>& Inst : CameraActionList)
	{
		TWeakObjectPtr<UCameraActionBase> CameraAction = Inst->CameraActionCache;
		if (!CameraAction.IsValid())
			continue;
	
		// CA打断检测
		ProcessInterruptPolicy(Inst);
		
		// 计时更新与结束判断
		if (CameraAction->HowToEnd == EFinishCameraAnimBy::FixedDuration && !CameraAction->bIsInfinityDurationTime)
		{
			if (Inst->CurrentState == ECameraActionState::Interrupted && CameraAction->bIsContinuous && !CameraAction->bUpdateTimeWhenInterrupted)
				continue;
			
			Inst->LeftEffectiveTime -= DeltaTime;
			if (Inst->LeftEffectiveTime <= 0.f)
				Inst->CurrentState = ECameraActionState::Finished;
		}
	}
}

void ULogicCameraActionManager::SortCameraActionList()
{
	// 优先级从高到低排序
	CameraActionList.Sort([](const TSharedPtr<FCameraActionInstance>& Left, const TSharedPtr<FCameraActionInstance>& Right) -> bool
	{
		return Left->Priority.Priority > Right->Priority.Priority;
	});

	// 得到所有待执行的相机行为
	bool bTrackCompletelyOccupied = false;
	CurFrameCameraAnimInstance.Reset();
	for (const TSharedPtr<FCameraActionInstance>& Inst : CameraActionList)
	{
		if (bTrackCompletelyOccupied)
		{
			InterruptCameraActionInternal(Inst);
			continue;
		}
		CurFrameCameraAnimInstance.Push(Inst);
		if (Inst->BindingInfo.bOccupyAllTracks)
			bTrackCompletelyOccupied = true;
	}
}

void ULogicCameraActionManager::FinishCameraActionInternal(const TSharedPtr<FCameraActionInstance>& InCameraActionInstance)
{
	if (InCameraActionInstance->CurrentState != ECameraActionState::Finished)
		return;

	InCameraActionInstance->BindingInfo.OnFinished.ExecuteIfBound();
	if (InCameraActionInstance->CameraActionCache.IsValid())
		InCameraActionInstance->CameraActionCache->Exit(CamMgrCache.Get());
	CameraTrackList->StopTracks(InCameraActionInstance->CameraActionCache.Get(), InCameraActionInstance->ActiveTracks);
	InCameraActionInstance->UnbindAllDelegates();
}

void ULogicCameraActionManager::InterruptCameraActionInternal(const TSharedPtr<FCameraActionInstance>& InCameraActionInstance)
{
	if (InCameraActionInstance->CameraActionCache.IsValid() && InCameraActionInstance->CameraActionCache->bIsContinuous)
	{
		if (InCameraActionInstance->CameraActionCache->bResumeByTrackOccupied)
		{
			InCameraActionInstance->BindingInfo.OnInterrupted.ExecuteIfBound();
			InCameraActionInstance->CameraActionCache->Interrupt(CamMgrCache.Get());
			InCameraActionInstance->CurrentState = ECameraActionState::Interrupted;
			return;
		}
	}

	InCameraActionInstance->CurrentState = ECameraActionState::Finished;
}

void ULogicCameraActionManager::EnterCameraActionInternal(const TSharedPtr<FCameraActionInstance>& InCameraActionInstance)
{
	if (!InCameraActionInstance->CameraActionCache.IsValid() || InCameraActionInstance->CurrentState != ECameraActionState::Awake)
		return;

	FCameraTrackValueCollection CurTrackValues;
	if (!CameraTrackList->GetCurrentTrackValues(CurTrackValues))
		return;

	FCameraTrackValueCollection OutTrackValues;
	uint16 ActiveTracks = InCameraActionInstance->CameraActionCache->Enter(CamMgrCache.Get(), CurTrackValues, OutTrackValues);
	CameraTrackList->ActiveTracks(InCameraActionInstance->CameraActionCache.Get(), ActiveTracks, OutTrackValues, InCameraActionInstance->GetPriority());
	InCameraActionInstance->ActiveTracks = ActiveTracks;
	InCameraActionInstance->BindingInfo.OnExecute.ExecuteIfBound();
	InCameraActionInstance->CurrentState = ECameraActionState::Executing;
}

void ULogicCameraActionManager::ResumeCameraActionInternal(const TSharedPtr<FCameraActionInstance>& InCameraActionInstance)
{
	if (!InCameraActionInstance->CameraActionCache.IsValid() || InCameraActionInstance->CurrentState != ECameraActionState::Interrupted)
		return;
	
	FCameraTrackValueCollection CurTrackValues;
	if (!CameraTrackList->GetCurrentTrackValues(CurTrackValues))
		return;
	
	InCameraActionInstance->CameraActionCache->Resume(CamMgrCache.Get());
	FCameraTrackValueCollection OutTrackValues;
	uint16 ActiveTracks = InCameraActionInstance->CameraActionCache->Enter(CamMgrCache.Get(), CurTrackValues, OutTrackValues);
	CameraTrackList->ActiveTracks(InCameraActionInstance->CameraActionCache.Get(), ActiveTracks, OutTrackValues, InCameraActionInstance->GetPriority());
	InCameraActionInstance->ActiveTracks = ActiveTracks;
	InCameraActionInstance->CurrentState = ECameraActionState::Executing;
	InCameraActionInstance->CurInterruptTag = FGameplayTag::EmptyTag;
}

void ULogicCameraActionManager::UpdateCameraActionInternal(const TSharedPtr<FCameraActionInstance>& InCameraActionInstance, float DeltaTime)
{
	if (!InCameraActionInstance->CameraActionCache.IsValid() || InCameraActionInstance->CurrentState != ECameraActionState::Executing)
		return;
	
	FCameraTrackValueCollection CurTrackValues;
	if (!CameraTrackList->GetCurrentTrackValues(CurTrackValues))
		return;

	FCameraTrackValueCollection OutTrackValues;
	uint16 ActiveTracks = InCameraActionInstance->CameraActionCache->Update(CamMgrCache.Get(), DeltaTime, CurTrackValues, OutTrackValues);
	CameraTrackList->UpdateTracks(InCameraActionInstance->CameraActionCache.Get(), ActiveTracks, OutTrackValues, InCameraActionInstance->GetPriority());
	InCameraActionInstance->ActiveTracks |= ActiveTracks;

	// 保底更新轨道占用（可能存在未实现Update但实现Enter的情况）
	uint16 OccupiedTracks = CameraTrackList->CheckTracksOccupy(InCameraActionInstance->ActiveTracks, InCameraActionInstance->GetPriority());
	InCameraActionInstance->ActiveTracks &= ~OccupiedTracks;

	// 当所有轨道被占用则标记为被打断
	if (InCameraActionInstance->ActiveTracks == 0)
	{
		InterruptCameraActionInternal(InCameraActionInstance);
		return;
	}

	// 当轨道值全部到达目标，则相机行为执行结束
	if (CameraTrackList->CheckTracksAllArrived(InCameraActionInstance->ActiveTracks))
	{
		if (InCameraActionInstance->CameraActionCache->HowToEnd == EFinishCameraAnimBy::ArrivedTargetPos)
		{
			InCameraActionInstance->CurrentState = ECameraActionState::Finished;
		}
	}
}

void ULogicCameraActionManager::ProcessInterruptPolicy(const TSharedPtr<FCameraActionInstance>& InCameraActionInstance)
{
	if (!CamMgrCache->LogicCameraSettings || !InCameraActionInstance->CameraActionCache.IsValid())
		return;
	
	TWeakObjectPtr<UCameraActionBase> CameraAction = InCameraActionInstance->CameraActionCache;
	
	for (const auto& InterruptPolicyPair : CamMgrCache->LogicCameraSettings->CameraActionInterruptPolicies)
	{
		TObjectPtr<UCameraActionInterruptPolicy> Policy = InterruptPolicyPair.Value.InterruptPolicy;
		if (!Policy)
			continue;
			
		// 若已激活的轨道不在打断策略检查范围内，无需继续检查
		if ((InCameraActionInstance->ActiveTracks & Policy->GetCheckTrackBitValue()) == 0)
			continue;
	
		// 检查是否可以打断
		bool bShouldInterrupt = Policy->CheckInterruptCondition(CamMgrCache.Get(), CameraAction.Get());
		
		if (CameraAction->bIsContinuous && CameraAction->ResumeByInterruptPolicyTags.HasTagExact(InterruptPolicyPair.Key))
		{
			if (bShouldInterrupt)
			{
				InCameraActionInstance->CurrentState = ECameraActionState::Interrupted;
				InCameraActionInstance->CurInterruptTag = InterruptPolicyPair.Key;
				InCameraActionInstance->BindingInfo.OnInterrupted.ExecuteIfBound();
				CameraAction->Interrupt(CamMgrCache.Get());
				CameraTrackList->StopTracks(CameraAction.Get(), InCameraActionInstance->ActiveTracks);
			}
			else
			{
				if (InCameraActionInstance->CurrentState == ECameraActionState::Interrupted &&
					InCameraActionInstance->CurInterruptTag == InterruptPolicyPair.Key)
				{
					ResumeCameraActionInternal(InCameraActionInstance);
				}
			}
		}
		else
		{
			if (bShouldInterrupt)
				InCameraActionInstance->CurrentState = ECameraActionState::Finished;
		}
	}
}
