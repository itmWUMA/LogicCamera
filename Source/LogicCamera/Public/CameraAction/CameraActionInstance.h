// Copyright ITMWUMA Victor, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"

class UCameraActionBase;

struct FCameraActionInstanceGenerateInfo
{
	FName PriorityName = TEXT("Default");
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
	FCameraActionInstance(const UCameraActionBase* Source, const FCameraActionInstanceGenerateInfo& InGenerateInfo,
		const FGuid& InID, uint32 MapPriority, uint32 DynamicPriority);
	bool operator==(const FCameraActionInstance& Rhs) const;

public:
	TWeakObjectPtr<const UCameraActionBase> CameraActionCache = nullptr;
	FGuid ID;
	FCameraActionInstanceGenerateInfo GenerateInfo;
	ECameraActionState CurrentState = ECameraActionState::Awake;
	CameraActionPriority Priority;
};
