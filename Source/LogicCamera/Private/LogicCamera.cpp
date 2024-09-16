// Copyright ITMWUMA Victor, 2024. All rights reserved.

#include "LogicCamera.h"

#include "CameraAction/LogicCameraActionManager.h"
#include "HAL/ConsoleManager.h"

#define LOCTEXT_NAMESPACE "FLogicCameraModule"

void FLogicCameraModule::StartupModule()
{
	FConsoleManager::Get().RegisterConsoleCommand(TEXT("LogicCamera.ShowTrackInfo"),
		TEXT("Show all camera tracks information"),
		FConsoleCommandDelegate::CreateStatic(&ULogicCameraActionManager::ShowTracksDebug),
		ECVF_Default);
}

void FLogicCameraModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLogicCameraModule, LogicCamera)