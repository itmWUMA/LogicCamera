// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "Core/LogicCameraStatics.h"

#include "ProxyCamera/CameraBlackboard.h"

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

float ULogicCameraStatics::GetFloatValueFromCameraBlackboard(const UCameraBlackboard* Blackboard, const FName& Key)
{
	if (!IsValid(Blackboard))
		return LC_INVALID_VALUE;

	return Blackboard->GetValueByName<float>(Key);
}

FVector ULogicCameraStatics::GetVectorValueFromCameraBlackboard(const UCameraBlackboard* Blackboard, const FName& Key)
{
	if (!IsValid(Blackboard))
		return FVector(LC_INVALID_VALUE);

	return Blackboard->GetValueByName<FVector>(Key);
}

UCurveFloat* ULogicCameraStatics::GetCurveValueFromCameraBlackboard(const UCameraBlackboard* Blackboard,
	const FName& Key)
{
	if (!IsValid(Blackboard))
		return nullptr;

	return Blackboard->GetValueByName<UCurveFloat*>(Key);
}
