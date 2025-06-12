// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "BaseCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "../Common/Finder.h"
#include "../Common/LogUtil.h"
#include "../Weapon/Weapon.h"
#include "../Weapon/Sword.h"

ABaseCharacter::ABaseCharacter() {
  PrimaryActorTick.bCanEverTick = true;

  bUseControllerRotationPitch = false;
  bUseControllerRotationYaw   = false;
  bUseControllerRotationRoll  = false;
}

void ABaseCharacter::BeginPlay() {
  Super::BeginPlay();
}

void ABaseCharacter::PostInitializeComponents() {
  Super::PostInitializeComponents();
}

void ABaseCharacter::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
}

void ABaseCharacter::NotifyControllerChanged() {
  Super::NotifyControllerChanged();
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled) {
  if (EquippedWeapon == nullptr) {
    return;
  }
  EquippedWeapon->IgnoreActors.Empty();

  auto* const WeaponBox = EquippedWeapon->GetWeaponBox();
  if (WeaponBox == nullptr) {
    return;
  }
  WeaponBox->SetCollisionEnabled(CollisionEnabled);
}

void ABaseCharacter::Die() {
}

void ABaseCharacter::PlayAttackMontage() {
}
