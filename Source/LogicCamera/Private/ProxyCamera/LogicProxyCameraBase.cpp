// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "ProxyCamera/LogicProxyCameraBase.h"

void ULogicProxyCameraBase::InitProxyCamera()
{
	ResetCameraBlackboard();
}

void ULogicProxyCameraBase::ResetCameraBlackboard()
{
	RuntimeCameraBlackboard = DefaultCameraBlackboard;
}
