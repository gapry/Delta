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
class UInputComponent;
class AWeapon;

UCLASS()
class DELTA_API ABaseCharacter : public ACharacter {
  GENERATED_BODY()

public:
  ABaseCharacter();

  virtual void Tick(float DeltaTime) override;

  virtual void NotifyControllerChanged() override;

  virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

  void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

protected:
  virtual void BeginPlay() override;

  virtual void PostInitializeComponents() override;

  virtual void Die();

  virtual void PlayAttackMontage();

  TWeakObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
  TWeakObjectPtr<UCapsuleComponent>      CapsuleComponent;
  TWeakObjectPtr<USpringArmComponent>    SpringArmComponent;
  TWeakObjectPtr<UCameraComponent>       CameraComponent;

  UPROPERTY(VisibleAnywhere, Category = "Weapon")
  TObjectPtr<AWeapon> EquippedWeapon;
};
