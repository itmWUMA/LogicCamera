// Copyright ITMWUMA Victor, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CameraActionInstance.h"
#include "Subsystems/WorldSubsystem.h"
#include "LogicCameraActionManager.generated.h"

class UCameraTrackList;
class ALogicPlayerCameraManager;
class UCameraActionBase;

/**
 * 相机行为的管理器
 *
 * 1. 存储相机轨道，并根据轨道值应用到主相机中，使得轨道值时机影响到相机表现
 * 2. 管理相机行为，处理相机行为间的关系，如结束、打断、恢复等
 */
UCLASS()
class LOGICCAMERA_API ULogicCameraActionManager : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	static ULogicCameraActionManager* Get(const UObject* WorldContext);
	
	void OnInit(ALogicPlayerCameraManager* LogicPlayerCameraManager);
	void OnReset();

	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

	// 生成相机行为实例，并添加到相机行为容器中进行管理
	FGuid AddCameraAction(UCameraActionBase* InCameraAction, const FCameraActionBindData& BindingInfo);

	// 查找相机实例
	FGuid FindCameraAction(UCameraActionBase* InCameraAction) const;
	TSharedPtr<FCameraActionInstance> FindCameraAction(const FGuid& InGuid) const;

	// 若已存在相机行为实例，则返回其ID，否则创建相机行为实例
	FGuid FindOrAddCameraAction(UCameraActionBase* InCameraAction, const FCameraActionBindData& BindingInfo);

	// 移除相机实例
	void RemoveCameraAction(const FGuid& InGuid);

private:
	// 生成CameraActionInstance，并赋予其唯一ID
	TSharedPtr<FCameraActionInstance> GenerateCameraActionInstance(UCameraActionBase* Source, const FCameraActionBindData& BindingInfo) const;
	int32 GetCameraActionMapPriority(const FCameraActionBindData& BindingInfo) const;
	static uint32 GetAndUpdateDynamicPriority();
	
	void UpdatePendingRemoveCameraAction();
	void UpdateCameraAction(float DeltaTime);
	
	void SortCameraActionList();
	void FinishCameraActionInternal(const TSharedPtr<FCameraActionInstance>& InCameraActionInstance);
	void InterruptCameraActionForTracks(const TSharedPtr<FCameraActionInstance>& InCameraActionInstance);
	void EnterCameraActionInternal(const TSharedPtr<FCameraActionInstance>& InCameraActionInstance);
	void ResumeCameraActionInternal(const TSharedPtr<FCameraActionInstance>& InCameraActionInstance);
	void UpdateCameraActionInternal(const TSharedPtr<FCameraActionInstance>& InCameraActionInstance, float DeltaTime);
	void ProcessInterruptPolicy(const TSharedPtr<FCameraActionInstance>& InCameraActionInstance);

private:
	// 相机实例的容器
	TArray<TSharedPtr<FCameraActionInstance>> CameraActionList;
	TObjectPtr<UCameraTrackList> CameraTrackList;
	TWeakObjectPtr<ALogicPlayerCameraManager> CamMgrCache = nullptr;
	TArray<TSharedPtr<FCameraActionInstance>> CurFrameCameraAnimInstance;
	TArray<TSharedPtr<FCameraActionInstance>> PreFrameCameraAnimInstance;
	
	static uint32 CameraActionDynamicPriority;
};
