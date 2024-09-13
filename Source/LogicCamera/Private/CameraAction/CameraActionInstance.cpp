// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "CameraAction/CameraActionInstance.h"
#include "CameraAction/CameraActionBase.h"

FCameraActionInstance::FCameraActionInstance(const UCameraActionBase* Source,
	const FCameraActionInstanceGenerateInfo& InGenerateInfo, const FGuid& InID, uint32 MapPriority,
	uint32 DynamicPriority)
{
	CameraActionCache = MakeWeakObjectPtr<const UCameraActionBase>(Source);
    GenerateInfo = InGenerateInfo;
    CurrentState = ECameraActionState::Awake;
    ID = InID;
	
	Priority.PriorityDetail.DynamicPriority = DynamicPriority;
	Priority.PriorityDetail.MapPriority = MapPriority;
}

bool FCameraActionInstance::operator==(const FCameraActionInstance& Rhs) const
{
	return ID == Rhs.ID;
}
