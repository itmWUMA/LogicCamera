// Copyright ITMWUMA Victor, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	UFUNCTION(BlueprintPure, Category = "Camera")
	AActor* GetFollowedTarget() const;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Logic Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Logic Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	TWeakObjectPtr<AActor> DefaultFollowActor = nullptr;
	TWeakObjectPtr<AActor> CurrentFollowActor = nullptr;
};
