// Copyright ITMWUMA Victor, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/LogicCameraDefines.h"
#include "CameraTrackList.generated.h"

UENUM(BlueprintType)
enum class ECameraTrackType : uint8
{
	Roll,
	Pitch,
	Yaw,
	ArmLength,
	SocketOffset_X,
	SocketOffset_Y,
	SocketOffset_Z,
	TargetOffset_X,
	TargetOffset_Y,
	TargetOffset_Z,
	FOV,
};

USTRUCT(BlueprintType)
struct LOGICCAMERA_API FCameraTrackValueCollection
{
	GENERATED_BODY()

	FCameraTrackValueCollection() = default;
	FCameraTrackValueCollection(float InValue) :
		Roll(InValue),
		Pitch(InValue),
		Yaw(InValue),
		ArmLength(InValue),
		SocketOffset_X(InValue),
		SocketOffset_Y(InValue),
		SocketOffset_Z(InValue),
		TargetOffset_X(InValue),
		TargetOffset_Y(InValue),
		TargetOffset_Z(InValue) { }
	
	
	bool Serialize(FArchive& Ar);

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float Roll = LC_INVALID_VALUE;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Pitch = LC_INVALID_VALUE;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Yaw = LC_INVALID_VALUE;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ArmLength = LC_INVALID_VALUE;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float  SocketOffset_X = LC_INVALID_VALUE;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SocketOffset_Y = LC_INVALID_VALUE;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SocketOffset_Z = LC_INVALID_VALUE;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TargetOffset_X = LC_INVALID_VALUE;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TargetOffset_Y = LC_INVALID_VALUE;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TargetOffset_Z = LC_INVALID_VALUE;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FOV = LC_INVALID_VALUE;
};

/**
 * 
 */
UCLASS()
class LOGICCAMERA_API UCameraTrackList : public UObject
{
	GENERATED_BODY()
	
};
