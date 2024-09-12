// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "Utilities/LogicCameraMathUtilities.h"

#include "Track/CameraTrackList.h"

uint16 FLogicCameraMathUtilities::GetTracksBitValueFromTrackType(const TArray<ECameraTrackType>& TracksType)
{
	uint16 SearchIndex = 1;
	uint16 TracksBitValue = 0;
	
	for (uint8 Index = 0; Index < static_cast<uint8>(ECameraTrackType::Max); ++Index)
	{
		if (TracksType.Contains(static_cast<ECameraTrackType>(Index)))
			TracksBitValue |= SearchIndex;
		SearchIndex <<= 1;
	}

	return TracksBitValue;
}
