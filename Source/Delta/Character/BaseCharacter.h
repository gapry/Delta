// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interface/HitInterface.h"
#include "BaseCharacter.generated.h"

#define DELTA_BASE_CHARACTER_ENABLE_DEBUG_HIT 0

class USkeletalMeshComponent;
class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputComponent;
class UAnimMontage;
class AWeapon;
class UAttributeComponent;
class USoundBase;
class UParticleSystem;

UCLASS()
class DELTA_API ABaseCharacter : public ACharacter, public IHitInterface {
  GENERATED_BODY()

public:
  ABaseCharacter();

  virtual void Tick(float DeltaTime) override;

  virtual void NotifyControllerChanged() override;

  virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

  void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

  virtual void GetHit(const FVector& ImpactPoint) override;

protected:
  virtual void BeginPlay() override;

  virtual void PostInitializeComponents() override;

  virtual void Die();

  virtual void PlayAttackMontage();

  void PlayHitReactMontage(const FName& SectionName);

  void DirectionalHitReact(const FVector& ImpactPoint);

  virtual bool CanAttack();

  TArray<FName> GetAllMontageSectionsNames(const UAnimMontage* const Montage);

  TWeakObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
  TWeakObjectPtr<UCapsuleComponent>      CapsuleComponent;
  TWeakObjectPtr<USpringArmComponent>    SpringArmComponent;
  TWeakObjectPtr<UCameraComponent>       CameraComponent;

  UPROPERTY(VisibleAnywhere, Category = "Weapon")
  TObjectPtr<AWeapon> EquippedWeapon;

  UPROPERTY(EditDefaultsOnly, Category = "Montage")
  TObjectPtr<UAnimMontage> AttackMontage;

  UPROPERTY(EditDefaultsOnly, Category = "Montages")
  TObjectPtr<UAnimMontage> HitReactMontage;

  UPROPERTY(EditDefaultsOnly, Category = "Montages")
  TObjectPtr<UAnimMontage> DeathMontage;

  UPROPERTY(VisibleAnywhere, Category = "Attributes")
  TObjectPtr<UAttributeComponent> AttributeComponent;

  UPROPERTY(EditAnywhere, Category = "Sounds")
  TObjectPtr<USoundBase> HitSound;

  UPROPERTY(EditAnywhere, Category = "VisualEffects")
  TObjectPtr<UParticleSystem> HitParticles;
};
