// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class DELTA_API ABaseCharacter : public ACharacter {
  GENERATED_BODY()

public:
  ABaseCharacter();

  virtual void Tick(float DeltaTime) override;

  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
  virtual void BeginPlay() override;

  TWeakObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
  TWeakObjectPtr<UCapsuleComponent>      CapsuleComponent;
  TWeakObjectPtr<USpringArmComponent>    SpringArmComponent;
  TWeakObjectPtr<UCameraComponent>       CameraComponent;
};
