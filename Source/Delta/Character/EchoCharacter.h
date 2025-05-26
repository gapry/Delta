// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "EchoCharacter.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UEnhancedInputLocalPlayerSubsystem;
class UGroomComponent;

UCLASS()
class DELTA_API AEchoCharacter : public ACharacter {
  GENERATED_BODY()

public:
  AEchoCharacter();

  virtual void NotifyControllerChanged() override;
  virtual void Tick(float DeltaTime) override;

  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  void Move(const FInputActionValue& Value);
  void Look(const FInputActionValue& Value);

protected:
  virtual void BeginPlay() override;
  virtual void PostInitializeComponents() override;

  void PostInitializeSkeletalMeshComponent();
  void PostInitializeCapsuleComponent();
  void PostInitializeSpringArmComponent();
  void PostInitializeCameraComponent();
  void PostInitializeCharacterMovementComponent();

private:
  APlayerController*                  GetPlayerController() const;
  UEnhancedInputLocalPlayerSubsystem* GetSubsystem() const;

  TWeakObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
  TWeakObjectPtr<UCapsuleComponent>      CapsuleComponent;
  TWeakObjectPtr<USpringArmComponent>    SpringArmComponent;
  TWeakObjectPtr<UCameraComponent>       CameraComponent;

  TObjectPtr<UInputMappingContext> InputMappingContext;
  TObjectPtr<UInputAction>         MoveAction;
  TObjectPtr<UInputAction>         LookAction;

  UPROPERTY(VisibleAnywhere,
            BlueprintReadOnly,
            Category = "EchoCharacter",
            meta     = (AllowPrivateAccess = "true"))
  TSoftObjectPtr<UGroomComponent> HairComponent;

  UPROPERTY(VisibleAnywhere,
            BlueprintReadOnly,
            Category = "EchoCharacter",
            meta     = (AllowPrivateAccess = "true"))
  TSoftObjectPtr<UGroomComponent> EyebrowsComponent;
};