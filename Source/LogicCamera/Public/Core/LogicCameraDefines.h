// Copyright ITMWUMA Victor, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LogicCameraDefines.generated.h"

UENUM(BlueprintType)
enum class ECameraTrackType : uint8
{
	Roll = 0,
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
	Max,
};

#define LC_INVALID_VALUE BIG_NUMBER
#define LC_CAMERA_TRACK_COUNT static_cast<uint8>(ECameraTrackType::Max)
#define LC_DEFAULT_CAMERA_ACTION_PRIORITY 100
#define LC_DEFAULT_CAMERA_ACTION_PRIORITY_NAME TEXT("Default")
#define LC_CAMERA_TRACK_DEBUG_CHANNEL 50000

LOGICCAMERA_API DECLARE_LOG_CATEGORY_EXTERN(LogLC, Log, All);

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

	float operator[](unsigned int Index) const;
	float& operator[](unsigned int Index);

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