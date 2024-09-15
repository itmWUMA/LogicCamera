// Copyright ITMWUMA Victor, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Track/CameraTrackList.h"
#include "UObject/NoExportTypes.h"
#include "CameraActionBase.generated.h"

class ALogicPlayerCameraManager;

USTRUCT(BlueprintType)
struct FCameraActionSpeedData
{
	GENERATED_BODY()

	// 轨道值插值时，是否使用ConstInterpTo，否则使用InterpTo
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseConstInterp = false;

	// 轨道值插值速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InterpSpeed = 1.f;

	// 乘方值，用于对插值曲线进行细调，乘方值越大初始轨道值变化更快
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!bUseConstInterp", EditConditionHides))
	float Exponent = 1.f;
};

UENUM(BlueprintType)
enum class EFinishCameraAnimBy: uint8
{
	// 相机动画播放固定时长
	FixedDuration,
	// 动画到达Target值
	ArrivedTargetPos
};

/**
 * 相机行为
 * 
 * 用于描述一段镜头效果（相机的行为），如旋转、拉近拉远、FOV调整等
 * 相机行为会控制8个轨道，根据相关接口（如Enter、Tick等）应用相应的轨道值，以完成相机行为的控制
 * 
 * 是整个相机框架中的最小单位
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew)
class LOGICCAMERA_API UCameraActionBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void Prepare(ALogicPlayerCameraManager* CamMgr);
	virtual uint16 Enter(ALogicPlayerCameraManager* CamMgr, const FCameraTrackValueCollection& CurTrackValues, OUT FCameraTrackValueCollection& OutTrackValues);
	virtual uint16 Update(ALogicPlayerCameraManager* CamMgr, float DeltaTime, const FCameraTrackValueCollection& CurTrackValues, OUT FCameraTrackValueCollection& OutTrackValues);
	virtual void Interrupt(ALogicPlayerCameraManager* CamMgr);
	virtual void Resume(ALogicPlayerCameraManager* CamMgr);
	virtual void Exit(ALogicPlayerCameraManager* CamMgr);

protected:
	// 执行CA前，用于初始化参数和状态设置
	UFUNCTION(BlueprintNativeEvent, Category = "Logic Camera|Camera Action")
	void OnPrepare(ALogicPlayerCameraManager* CamMgr);
	virtual void OnPrepare_Implementation(ALogicPlayerCameraManager* CamMgr) { }

	// 开始执行CA
	UFUNCTION(BlueprintNativeEvent, Category = "Logic Camera|Camera Action")
	void OnEnter(ALogicPlayerCameraManager* CamMgr, const FCameraTrackValueCollection& CurTrackValues, FCameraTrackValueCollection& OutTrackValues);
	virtual void OnEnter_Implementation(ALogicPlayerCameraManager* CamMgr, const FCameraTrackValueCollection& CurTrackValues, FCameraTrackValueCollection& OutTrackValues) { }

	// 执行CA的过程中
	UFUNCTION(BlueprintNativeEvent, Category = "Logic Camera|Camera Action")
	void OnUpdate(ALogicPlayerCameraManager* CamMgr, float DeltaTime, const FCameraTrackValueCollection& CurTrackValues, FCameraTrackValueCollection& OutTrackValues);
	virtual void OnUpdate_Implementation(ALogicPlayerCameraManager* CamMgr, float DeltaTime, const FCameraTrackValueCollection& CurTrackValues, FCameraTrackValueCollection& OutTrackValues) { }

	// 打断CA，需要开启持续相机行为属性
	UFUNCTION(BlueprintNativeEvent, Category = "Logic Camera|Camera Action")
	void OnInterrupt(ALogicPlayerCameraManager* CamMgr);
	virtual void OnInterrupt_Implementation(ALogicPlayerCameraManager* CamMgr) { }

	// 恢复CA，需要开启持续相机行为属性
	UFUNCTION(BlueprintNativeEvent, Category = "Logic Camera|Camera Action")
	void OnResume(ALogicPlayerCameraManager* CamMgr);
	virtual void OnResume_Implementation(ALogicPlayerCameraManager* CamMgr) { }

	// 退出CA
	UFUNCTION(BlueprintNativeEvent, Category = "Logic Camera|Camera Action")
	void OnExit(ALogicPlayerCameraManager* CamMgr);
	virtual void OnExit_Implementation(ALogicPlayerCameraManager* CamMgr) { }

private:
	// 逐轨道计算理论轨道值，以及已经激活的轨道ID
	uint16 GetTrackValue(const FCameraTrackValueCollection& TempOutTrackValues, FCameraTrackValueCollection& OutTrackValues);

public:
	// 插值速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Track|Speed")
	FCameraActionSpeedData InterpSpeedData;

	UPROPERTY(EditAnywhere, Category = "Track|Speed", meta = (InlineEditConditionToggle))
	bool bUseDownInterpSpeed = false;

	// 下行插值速度，当前值大于目标值时则使用该速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Track|Speed", meta = (EditCondition = "bUseDownInterpSpeed"))
	FCameraActionSpeedData DownInterpSpeedData;

	// 容差值，当前值距目标值小于容差值时，则认为到达目标
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Track|End", meta = (ClampMin = 0.1f, UIMin = 0.1f, ClampMax = 1.f, UIMax = 1.f))
	float Tolerance = 1.f;

	// 相机结束方式
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Track|End")
	EFinishCameraAnimBy HowToEnd = EFinishCameraAnimBy::ArrivedTargetPos;

	// 是否持续时间无限
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Track|End", meta = (EditCondition = "HowToEnd == EFinishCameraAnimBy::FixedDuration", EditConditionHides))
	bool bIsInfinityDurationTime = false;

	// 持续时间，执行超出时间后即退出
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Track|End", meta = (EditCondition = "HowToEnd == EFinishCameraAnimBy::FixedDuration && !bIsInfinityDurationTime", EditConditionHides))
	float Duration = 1.f;

	// 是否为持续相机行为，即相机行为被打断后能否重新恢复
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Continuous")
	bool bIsContinuous = false;

	// 当相机行为因轨道占用（被其他CA打断）而恢复时，是否重新执行相机行为
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Continuous", meta = (EditCondition = "bIsContinuous", EditConditionHides))
	bool bResumeByTrackOccupied = true;

	// 当相机行为因其他扩展打断策略而恢复时，是否重新执行相机行为（扩展打断策略见LogicCameraDataConfig）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Continuous", meta = (EditCondition = "bIsContinuous", EditConditionHides))
	FGameplayTagContainer ResumeByInterruptPolicyTags;

	// 当相机行为被打断时是否仍更新时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Continuous", meta = (EditCondition = "bIsContinuous", EditConditionHides))
	bool bUpdateTimeWhenInterrupted = true;
};
