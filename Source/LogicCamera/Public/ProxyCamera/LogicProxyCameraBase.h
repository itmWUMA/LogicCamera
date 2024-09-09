// Copyright ITMWUMA Victor, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LogicProxyCameraBase.generated.h"

class UCameraBlackboard;

/**
 * 代理相机（虚拟相机）
 *
 * 有别于MainCamera，MainCamera用于实际应用相机参数，以调整相机的效果
 * ProxyCamera用于组织相机间的逻辑，以在相应的时机内切换相应的相机行为（CA）
 * 可以使用状态机或行为树进一步对相机的行为进行组织管理
 *
 * 如第一人称相机、第三人称相机、镜头跟随相机、轨道相机等
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class LOGICCAMERA_API ULogicProxyCameraBase : public UObject
{
	GENERATED_BODY()

public:
	void InitProxyCamera();

	void ResetCameraBlackboard();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Logic Camera|Blackboard")
	TObjectPtr<UCameraBlackboard> DefaultCameraBlackboard;

private:
	TObjectPtr<UCameraBlackboard> RuntimeCameraBlackboard;
};
