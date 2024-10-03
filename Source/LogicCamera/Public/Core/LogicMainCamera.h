// Copyright ITMWUMA Victor, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Track/CameraTrack.h"
#include "LogicMainCamera.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * 主相机，根据轨道值调整相机相关参数
 * 包含CameraComponent和SpringArmComponent
 */
UCLASS()
class LOGICCAMERA_API ALogicMainCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	ALogicMainCamera();
	virtual void Tick(float DeltaSeconds) override;
	virtual void BecomeViewTarget(class APlayerController* PC) override;
	virtual void EndViewTarget(class APlayerController* PC) override;

	void ApplyTrackValue(const FCameraTrack& Track);
	bool CollectCurrentTrackValues(FCameraTrackValueCollection& OutParams) const;

	UFUNCTION(BlueprintPure, Category = "Camera")
	AActor* GetFollowedTarget() const;

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void StopFollowCamera();

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void ResetFollowCameraToDefault();

	UFUNCTION(BlueprintPure, Category = "Camera")
	FRotator GetCurrentViewRotation() const;

protected:
	virtual void BeginPlay() override;

private:
	void ApplyRotation(const FCameraTrack& Track);
	void ApplySpringArm(const FCameraTrack& Track);
	void ApplyCameraComponent(const FCameraTrack& Track);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Logic Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Logic Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	TWeakObjectPtr<AActor> DefaultFollowActor = nullptr;
	TWeakObjectPtr<AActor> CurrentFollowActor = nullptr;
};
