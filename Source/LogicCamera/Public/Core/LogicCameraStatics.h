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
	UFUNCTION(BlueprintPure, Category = "Logic Camera|Track")
	static float GetValueFromTrackCollecton(const FCameraTrackValueCollection& TrackValues, ECameraTrackType TrackType);

	UFUNCTION(BlueprintPure, Category = "Logic Camera|Blackboard", meta = (AutoCreateRefTerm = "Key"))
	static float GetFloatValueFromCameraBlackboard(const UCameraBlackboard* Blackboard, const FName& Key);

	UFUNCTION(BlueprintPure, Category = "Logic Camera|Blackboard", meta = (AutoCreateRefTerm = "Key"))
	static FVector GetVectorValueFromCameraBlackboard(const UCameraBlackboard* Blackboard, const FName& Key);

	UFUNCTION(BlueprintPure, Category = "Logic Camera|Blackboard", meta = (AutoCreateRefTerm = "Key"))
	static UCurveFloat* GetCurveValueFromCameraBlackboard(const UCameraBlackboard* Blackboard, const FName& Key);
};
