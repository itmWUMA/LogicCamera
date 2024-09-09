// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "ProxyCamera/CameraBlackboard.h"

#include "Core/LogicCameraDefines.h"

float UCameraBlackboard::GetFloatValueByName(const FName& Key) const
{
	if (const float* Ptr = FloatCollections.Find(Key))
		return *Ptr;
	return LC_INVALID_VALUE;
}

FVector UCameraBlackboard::GetVectorValueByName(const FName& Key) const
{
	if (const FVector* Ptr = VectorCollections.Find(Key))
		return  *Ptr;
	return FVector(LC_INVALID_VALUE);
}

UCurveFloat* UCameraBlackboard::GetCurveByName(const FName& Key) const
{
	if (auto* Ptr = FloatCurveCollections.Find(Key))
		return IsValid(*Ptr) ? *Ptr : nullptr;
	return nullptr;
}
