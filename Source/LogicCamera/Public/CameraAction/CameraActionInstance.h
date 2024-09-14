// Copyright ITMWUMA Victor, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/LogicCameraDefines.h"

class UCameraActionBase;

struct FCameraActionBindData
{
	FName PriorityName = LC_DEFAULT_CAMERA_ACTION_PRIORITY_NAME;

	// 生命周期回调，当运行至CA相应的生命周期后会触发
	FSimpleDelegate OnExecute;
	FSimpleDelegate OnInterrupted;
	FSimpleDelegate OnFinished;
};

/**
 * 优先比较优先级表的优先级，当优先级一致则比较动态优先级
 */
union CameraActionPriority
{
	struct PriorityDetails
	{
		uint32 DynamicPriority;
		uint32 MapPriority;
	}PriorityDetail;

	uint64 Priority;
	void operator=(uint64 InPriority) { Priority = InPriority; }
};

enum class ECameraActionState
{
	Awake,
	Executing,
	Interrupted,
	Finished,
};

/**
 * 相机行为的包装类，用于处理轨道值以外的内部逻辑
 * 如优先级、剩余时间等
 */
class LOGICCAMERA_API FCameraActionInstance
{
public:
	FCameraActionInstance(UCameraActionBase* Source, const FCameraActionBindData& InBindingInfo,
		const FGuid& InID, uint32 MapPriority, uint32 DynamicPriority);
	bool operator==(const FCameraActionInstance& Rhs) const;

	void UnbindAllDelegates();

public:
	TWeakObjectPtr<UCameraActionBase> CameraActionCache = nullptr;
	FGuid ID;
	FCameraActionBindData BindingInfo;
	ECameraActionState CurrentState = ECameraActionState::Awake;
	CameraActionPriority Priority;
	uint16 ActiveTracks = 0;
};
