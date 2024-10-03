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

void ALogicMainCamera::ApplyTrackValue(const FCameraTrack& Track)
{
	ApplyRotation(Track);
	ApplySpringArm(Track);
	ApplyCameraComponent(Track);
}

bool ALogicMainCamera::CollectCurrentTrackValues(FCameraTrackValueCollection& OutParams) const
{
	if (!SpringArmComponent || !CameraComponent)
		return false;
	
	FRotator CurCamRotation = GetCurrentViewRotation();
	OutParams.Pitch = CurCamRotation.Pitch;
	OutParams.Yaw = CurCamRotation.Yaw;
	OutParams.Roll = CurCamRotation.Roll;

	OutParams.ArmLength = SpringArmComponent->TargetArmLength;
	OutParams.SocketOffset_X = SpringArmComponent->SocketOffset.X;
	OutParams.SocketOffset_Y = SpringArmComponent->SocketOffset.Y;
	OutParams.SocketOffset_Z = SpringArmComponent->SocketOffset.Z;
	OutParams.TargetOffset_X = SpringArmComponent->TargetOffset.X;
	OutParams.TargetOffset_Y = SpringArmComponent->TargetOffset.Y;
	OutParams.TargetOffset_Z = SpringArmComponent->TargetOffset.Z;

	OutParams.FOV = CameraComponent->FieldOfView;

	return true;
}

AActor* ALogicMainCamera::GetFollowedTarget() const
{
	return CurrentFollowActor.Get();
}

void ALogicMainCamera::StopFollowCamera()
{
	CurrentFollowActor.Reset();
}

void ALogicMainCamera::ResetFollowCameraToDefault()
{
	CurrentFollowActor = DefaultFollowActor;
}

FRotator ALogicMainCamera::GetCurrentViewRotation() const
{
	return SpringArmComponent->GetComponentRotation();
}

void ALogicMainCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALogicMainCamera::ApplyRotation(const FCameraTrack& Track)
{
	FRotator CurrentRotation = GetCurrentViewRotation();
	
	switch (static_cast<ECameraTrackType>(Track.TrackID))
	{
	case ECameraTrackType::Pitch:
		CurrentRotation.Pitch = Track.CurValue;
		break;
	case ECameraTrackType::Yaw:
		CurrentRotation.Yaw = Track.CurValue;
		break;
	case ECameraTrackType::Roll:
		CurrentRotation.Roll = Track.CurValue;
		break;
	default:
		return;	
	}

	SpringArmComponent->SetWorldRotation(CurrentRotation);
}

void ALogicMainCamera::ApplySpringArm(const FCameraTrack& Track)
{
	switch (static_cast<ECameraTrackType>(Track.TrackID))
	{
	case ECameraTrackType::ArmLength:
		SpringArmComponent->TargetArmLength = Track.CurValue;
		break;
	case ECameraTrackType::SocketOffset_X:
		SpringArmComponent->SocketOffset.X = Track.CurValue;
		break;
	case ECameraTrackType::SocketOffset_Y:
		SpringArmComponent->SocketOffset.Y = Track.CurValue;
		break;
	case ECameraTrackType::SocketOffset_Z:
		SpringArmComponent->SocketOffset.Z = Track.CurValue;
		break;
	case ECameraTrackType::TargetOffset_X:
		SpringArmComponent->TargetOffset.X = Track.CurValue;
		break;
	case ECameraTrackType::TargetOffset_Y:
		SpringArmComponent->TargetOffset.Y = Track.CurValue;
		break;
	case ECameraTrackType::TargetOffset_Z:
		SpringArmComponent->TargetOffset.Z = Track.CurValue;
		break;
	default:
		return;
	}
}

void ALogicMainCamera::ApplyCameraComponent(const FCameraTrack& Track)
{
	switch (static_cast<ECameraTrackType>(Track.TrackID))
	{
	case ECameraTrackType::FOV:
		CameraComponent->FieldOfView = Track.CurValue;
		break;
	default:
		return;
	}
}
