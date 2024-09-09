// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "Track/CameraTrackList.h"

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
