// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "Core/LogicCameraStatics.h"

float ULogicCameraStatics::GetValueFromTrackCollecton(const FCameraTrackValueCollection& TrackValues,
	ECameraTrackType TrackType)
{
	uint8 Index = 0;
	for (TFieldIterator<FFloatProperty> PropIter(FCameraTrackValueCollection::StaticStruct()); PropIter; ++PropIter)
	{
		FFloatProperty* Prop = CastField<FFloatProperty>(*PropIter);
		if (Index == static_cast<uint8>(TrackType))
			return *Prop->ContainerPtrToValuePtr<float>(&TrackValues);
		++Index;
	}

	return LC_INVALID_VALUE;
}
