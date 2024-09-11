// Copyright ITMWUMA Victor, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class LOGICCAMERA_API FLogicCameraMathUtilities
{
public:
	template<typename T1, typename T2 , typename T3 , typename T4 >
	static auto TrackInterpTo(const T1& Current, const T2& Target, T3 DeltaTime, T4 InterpSpeed, float Exponent);
};

template <typename T1, typename T2, typename T3, typename T4>
FORCEINLINE auto FLogicCameraMathUtilities::TrackInterpTo(const T1& Current, const T2& Target, T3 DeltaTime, T4 InterpSpeed,
	float Exponent)
{
	using RetType = decltype(T1()* T2()* T3()* T4());

	if (InterpSpeed < 0.f)
	{
		return static_cast<RetType>(Target);
	}

	const RetType Dist = Target - Current;

	if (FMath::Square(Dist) < SMALL_NUMBER)
	{
		return static_cast<RetType>(Target);
	}

	const RetType TempDeltaMove = FMath::Sign(Dist) * FMath::Pow(FMath::Abs(Dist), Exponent) * DeltaTime * InterpSpeed;
	const RetType DeltaMove = FMath::Sign(Dist) * FMath::Clamp<RetType>(FMath::Abs(TempDeltaMove), 0, FMath::Abs(Dist));

	return Current + DeltaMove;
}
