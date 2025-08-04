// Copyright ITMWUMA Victor, 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CameraAction/CameraActionInstance.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "ExecuteCameraActionAsyncNode.generated.h"

class UCameraActionBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCameraActionAsyncEvent);

/**
 * 触发一段相机行为
 */
UCLASS(MinimalAPI)
class UExecuteCameraActionAsyncNode : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName = "Execute Camera Action", Category = "Logic Camera", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContext"))
	static UExecuteCameraActionAsyncNode* CreateExecuteCameraActionAsyncNode(
		const UObject* WorldContext,
		UCameraActionBase* CameraAction,
		FName Priority = NAME_None,
		bool bOccupyAllTracks = false);

private:
	void BindAsyncEvent(FCameraActionBindData& BindData);

public:
	UPROPERTY(BlueprintAssignable)
	FOnCameraActionAsyncEvent OnExecute;

	UPROPERTY(BlueprintAssignable)
	FOnCameraActionAsyncEvent OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FOnCameraActionAsyncEvent OnFinished;
};
