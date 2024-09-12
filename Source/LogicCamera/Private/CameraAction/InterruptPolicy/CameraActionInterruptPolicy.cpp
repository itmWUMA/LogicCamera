// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "CameraAction/InterruptPolicy/CameraActionInterruptPolicy.h"

#include "Utilities/LogicCameraMathUtilities.h"

bool UCameraActionInterruptPolicy::CheckInterruptCondition_Implementation(ALogicPlayerCameraManager* CamMgr,
                                                                          UCameraActionBase* CheckingCameraAction) const
{
	return false;
}

uint16 UCameraActionInterruptPolicy::GetCheckTrackBitValue() const
{
	return FLogicCameraMathUtilities::GetTracksBitValueFromTrackType(CheckTrackTypes);
}
