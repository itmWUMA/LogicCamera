// Copyright ITMWUMA Victor, 2024. All rights reserved.


#include "Core/LogicMainCamera.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ALogicMainCamera::ALogicMainCamera()
{
	PrimaryActorTick.bCanEverTick = true;

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

void ALogicMainCamera::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentFollowActor.IsValid())
	{
		SetActorLocation(CurrentFollowActor->GetActorLocation());
	}
}

void ALogicMainCamera::BecomeViewTarget(class APlayerController* PC)
{
	Super::BecomeViewTarget(PC);

	if (IsValid(PC))
	{
		DefaultFollowActor = PC->GetPawn();
		CurrentFollowActor = DefaultFollowActor;
		
	}
}

void ALogicMainCamera::EndViewTarget(class APlayerController* PC)
{
	DefaultFollowActor.Reset();
	CurrentFollowActor.Reset();
	
	Super::EndViewTarget(PC);
}

AActor* ALogicMainCamera::GetFollowedTarget() const
{
	return CurrentFollowActor.Get();
}

void ALogicMainCamera::BeginPlay()
{
	Super::BeginPlay();
	
}
