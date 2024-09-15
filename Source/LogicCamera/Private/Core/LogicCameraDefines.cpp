// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "Core/LogicCameraDefines.h"

#include "Core/LogicCameraStatics.h"

DEFINE_LOG_CATEGORY(LogLC);

bool FCameraTrackValueCollection::Serialize(FArchive& Ar)
{
	Ar << Roll;
	Ar << Pitch;
	Ar << Yaw;
	Ar << ArmLength;
	Ar << SocketOffset_X;
	Ar << SocketOffset_Y;
	Ar << SocketOffset_Z;
	Ar << TargetOffset_X;
	Ar << TargetOffset_Y;
	Ar << TargetOffset_Z;
	Ar << FOV;
	return true;
}

float FCameraTrackValueCollection::operator[](unsigned int Index) const
{
	ensureMsgf(Index <= LC_CAMERA_TRACK_COUNT - 1, TEXT("Invalid track index [%d]"), Index);
	return ULogicCameraStatics::GetValueFromTrackCollection(*this, static_cast<ECameraTrackType>(Index));
}

float& FCameraTrackValueCollection::operator[](unsigned int Index)
{
	ensureMsgf(Index <= LC_CAMERA_TRACK_COUNT - 1, TEXT("Invalid track index [%d]"), Index);
	uint8 SearchIndex = 0;
	for (TFieldIterator<FFloatProperty> i(FCameraTrackValueCollection::StaticStruct()); i; ++i)
	{
		FFloatProperty* prop = CastField<FFloatProperty>(*i);
		if (SearchIndex == Index) return *prop->ContainerPtrToValuePtr<float>(this);
		SearchIndex++;
	}

	float* ErrorReturn = nullptr;
	return *ErrorReturn;
}