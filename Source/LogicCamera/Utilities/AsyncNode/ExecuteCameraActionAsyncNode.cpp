// Copyright ITMWUMA Victor, 2025. All rights reserved.


#include "ExecuteCameraActionAsyncNode.h"

#include "CameraAction/LogicCameraActionManager.h"

UExecuteCameraActionAsyncNode* UExecuteCameraActionAsyncNode::CreateExecuteCameraActionAsyncNode(
	const UObject* WorldContext, UCameraActionBase* CameraAction, FName Priority, bool bOccupyAllTracks)
{
	UExecuteCameraActionAsyncNode* Node = nullptr;
	
	if (IsValid(WorldContext) && IsValid(WorldContext->GetWorld()))
	{
		Node = NewObject<UExecuteCameraActionAsyncNode>();
		Node->RegisterWithGameInstance(WorldContext);

		if (ULogicCameraActionManager* CameraActionMgr = WorldContext->GetWorld()->GetSubsystem<ULogicCameraActionManager>())
		{
			FCameraActionBindData BindData;
			BindData.PriorityName = Priority == NAME_None ? LC_DEFAULT_CAMERA_ACTION_PRIORITY_NAME : Priority;
			BindData.bOccupyAllTracks = bOccupyAllTracks;
			Node->BindAsyncEvent(BindData);
			CameraActionMgr->FindOrAddCameraAction(CameraAction, BindData);
		}
	}
	
	return Node;
}

void UExecuteCameraActionAsyncNode::BindAsyncEvent(FCameraActionBindData& BindData)
{
	BindData.OnExecute.BindWeakLambda(this, [this]()
	{
		OnExecute.Broadcast();
	});
	BindData.OnInterrupted.BindWeakLambda(this, [this]
	{
		OnInterrupted.Broadcast();
	});
	BindData.OnFinished.BindWeakLambda(this, [this]
	{
		OnFinished.Broadcast();
	});
}
