// Copyright ITMWUMA Victor, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Track/CameraTrackList.h"
#include "LogicCameraStatics.generated.h"

class UCameraBlackboard;

/**
 * 
 */
UCLASS()
class LOGICCAMERA_API ULogicCameraStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 根据轨道类型选取对应的轨道值
	UFUNCTION(BlueprintPure, Category = "Logic Camera|Track")
	static float GetValueFromTrackCollecton(const FCameraTrackValueCollection& TrackValues, ECameraTrackType TrackType);

	// 相机黑板取Float值
	UFUNCTION(BlueprintPure, Category = "Logic Camera|Blackboard", meta = (AutoCreateRefTerm = "Key"))
	static float GetFloatValueFromCameraBlackboard(const UCameraBlackboard* Blackboard, const FName& Key);

	// 相机黑板取Vector值
	UFUNCTION(BlueprintPure, Category = "Logic Camera|Blackboard", meta = (AutoCreateRefTerm = "Key"))
	static FVector GetVectorValueFromCameraBlackboard(const UCameraBlackboard* Blackboard, const FName& Key);

	// 相机黑板取Curve
	UFUNCTION(BlueprintPure, Category = "Logic Camera|Blackboard", meta = (AutoCreateRefTerm = "Key"))
	static UCurveFloat* GetCurveValueFromCameraBlackboard(const UCameraBlackboard* Blackboard, const FName& Key);
};
