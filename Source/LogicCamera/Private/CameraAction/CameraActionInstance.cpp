// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "CameraAction/CameraActionInstance.h"
#include "CameraAction/CameraActionBase.h"

FCameraActionInstance::FCameraActionInstance(UCameraActionBase* Source,
	const FCameraActionBindData& InBindingInfo, const FGuid& InID, uint32 MapPriority,
	uint32 DynamicPriority)
{
	CameraActionCache = MakeWeakObjectPtr<UCameraActionBase>(Source);
    BindingInfo = InBindingInfo;
    CurrentState = ECameraActionState::Awake;
    ID = InID;
	
	Priority.PriorityDetail.DynamicPriority = DynamicPriority;
	Priority.PriorityDetail.MapPriority = MapPriority;
}

bool FCameraActionInstance::operator==(const FCameraActionInstance& Rhs) const
{
	return ID == Rhs.ID;
}

void FCameraActionInstance::BindParams()
{
	if (!CameraActionCache.IsValid())
	{
		UE_LOG(LogLC, Warning, TEXT("Invalid camera action cache when binding params(FCameraActionInstance[%s])"), *ID.ToString());
		return;
	}
	LeftEffectiveTime = CameraActionCache->bIsInfinityDurationTime ? LC_INVALID_VALUE : CameraActionCache->Duration;
}

void FCameraActionInstance::UnbindAllDelegates()
{
	BindingInfo.OnExecute.Unbind();
	BindingInfo.OnFinished.Unbind();
	BindingInfo.OnInterrupted.Unbind();
}
