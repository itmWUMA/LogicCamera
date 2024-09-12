// Copyright ITMWUMA Victor, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/LogicCameraDefines.h"
#include "UObject/NoExportTypes.h"
#include "CameraActionInterruptPolicy.generated.h"

class UCameraActionBase;
class ALogicPlayerCameraManager;

/**
 * 相机的打断策略，用于扩展打断相机行为的情况，如输入等
 * 需要实现打断检查接口（CheckInterruptCondition）
 */
UCLASS(Abstract)
class LOGICCAMERA_API UCameraActionInterruptPolicy : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	bool CheckInterruptCondition(ALogicPlayerCameraManager* CamMgr, UCameraActionBase* CheckingCameraAction) const;
	virtual bool CheckInterruptCondition_Implementation(ALogicPlayerCameraManager* CamMgr, UCameraActionBase* CheckingCameraAction) const;

	uint16 GetCheckTrackBitValue() const;

protected:
	// 打断轨道检测，打断检测只会针对选择的轨道
	UPROPERTY(EditAnywhere, Category = "Track")
	TArray<ECameraTrackType> CheckTrackTypes;
};
