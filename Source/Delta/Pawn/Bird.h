// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Bird.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCapsuleComponent;
class UInputMappingContext;
class UInputAction;
class UFloatingPawnMovement;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class DELTA_API ABird : public APawn {
  GENERATED_BODY()

public:
  ABird();

  virtual void Tick(float DeltaTime) override;

  virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

  void PostInitializeSkeletalMeshComponent();
  void PostInitializeCapsuleComponent();
  void PostInitializeCollision();
  void PostInitializeFloatingPawnMovement();
  void PostInitializeSpringArmComponent();
  void PostInitializeCameraComponent();

protected:
  virtual void BeginPlay() override;
  virtual void PostInitializeComponents() override;

  void Move(const FInputActionValue& Value);

private:
  UPROPERTY(VisibleAnywhere,
            BlueprintReadOnly,
            Category = "Bird",
            meta     = (AllowPrivateAccess = "true"))
  TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent{nullptr};

  UPROPERTY(VisibleAnywhere,
            BlueprintReadOnly,
            Category = "Bird",
            meta     = (AllowPrivateAccess = "true"))
  TObjectPtr<UCapsuleComponent> CapsuleComponent{nullptr};

  UPROPERTY(VisibleAnywhere, Category = "Bird")
  TObjectPtr<UInputMappingContext> InputMappingContext{nullptr};

  UPROPERTY(VisibleAnywhere, Category = "Bird")
  TObjectPtr<UInputAction> MoveAction{nullptr};

  UPROPERTY(VisibleAnywhere, Category = "Bird")
  TObjectPtr<UFloatingPawnMovement> FloatingPawnMovement{nullptr};

  UPROPERTY(VisibleAnywhere, Category = "Bird")
  TObjectPtr<USpringArmComponent> SpringArmComponent{nullptr};

  UPROPERTY(VisibleAnywhere, Category = "Bird")
  TObjectPtr<UCameraComponent> CameraComponent{nullptr};
};
