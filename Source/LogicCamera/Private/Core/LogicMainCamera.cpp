// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "Core/LogicMainCamera.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ALogicMainCamera::ALogicMainCamera()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->bEnableCameraRotationLag = true;
	SpringArmComponent->CameraLagSpeed = 4;
	SpringArmComponent->CameraRotationLagSpeed = 5;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ALogicMainCamera::BeginPlay()
{
	Super::BeginPlay();
	
}
