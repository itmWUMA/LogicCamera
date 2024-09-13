// Copyright ITMWUMA Victor, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "LogicDataConfig.generated.h"

class UCameraActionInterruptPolicy;

USTRUCT()
struct FCameraActionInterruptPolicyData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UCameraActionInterruptPolicy> InterruptPolicy;
};

/**
 * 
 */
UCLASS()
class LOGICCAMERA_API ULogicDataConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	ULogicDataConfig();

public:
	UPROPERTY(EditAnywhere, Category = "Camera Action")
	TMap<FGameplayTag, FCameraActionInterruptPolicyData> CameraActionInterruptPolicies;

	UPROPERTY(EditAnywhere, Category = "Camera Action", meta = (ClampMin = 0, UIMin = 0))
	TMap<FName, int32> CameraActionPriorityMap;
};
