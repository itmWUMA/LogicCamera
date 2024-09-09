// Copyright ITMWUMA Victor, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CameraBlackboard.generated.h"

/**
 * 相机的参数集，用于存储相机所需的一些配置信息，存储于代理相机中
 */
UCLASS()
class LOGICCAMERA_API UCameraBlackboard : public UDataAsset
{
	GENERATED_BODY()

public:
	template<typename T>
	T GetValueByName(const FName& Key) const;

private:
	float GetFloatValueByName(const FName& Key) const;
	FVector GetVectorValueByName(const FName& Key) const;
	UCurveFloat* GetCurveByName(const FName& Key) const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, float> FloatCollections;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, FVector> VectorCollections;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, UCurveFloat*> FloatCurveCollections;
};

template <>
FORCEINLINE float UCameraBlackboard::GetValueByName<float>(const FName& Key) const
{
	return GetFloatValueByName(Key);
}

template<>
FORCEINLINE FVector UCameraBlackboard::GetValueByName<FVector>(const FName& Key) const
{
	return GetVectorValueByName(Key);
}

template<>
FORCEINLINE UCurveFloat* UCameraBlackboard::GetValueByName<UCurveFloat*>(const FName& Key) const
{
	return GetCurveByName(Key);
}