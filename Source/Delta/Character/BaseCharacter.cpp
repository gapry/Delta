// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "BaseCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../Component/AttributeComponent.h"
#include "../Common/Finder.h"
#include "../Common/LogUtil.h"
#include "../Weapon/Weapon.h"
#include "../Weapon/Sword.h"

ABaseCharacter::ABaseCharacter() {
  {
    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw   = false;
    bUseControllerRotationRoll  = false;
  }

  {
    AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
  }
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

void ABaseCharacter::GetHit(const FVector& ImpactPoint) {
}

bool ABaseCharacter::IsAlive() const {
  return AttributeComponent != nullptr && AttributeComponent->GetHealth() > 0.0f;
}

void ABaseCharacter::Die() {
}

void ABaseCharacter::PlayAttackMontage() {
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName) {
  UAnimInstance* const AnimInstance = GetMesh()->GetAnimInstance();
  if (AnimInstance && HitReactMontage) {
    AnimInstance->Montage_Play(HitReactMontage);
    AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
  }
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint) {
  const FVector Forward = GetActorForwardVector();

  const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
  const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

  const double CosTheta = FVector::DotProduct(Forward, ToHit);
  double       Theta    = FMath::RadiansToDegrees(FMath::Acos(CosTheta));

  const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
  if (CrossProduct.Z < 0) {
    Theta *= -1.f;
  }

#if DELTA_BASE_CHARACTER_ENABLE_DEBUG_HIT
  UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);
  UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
  UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f, FColor::Blue, 5.f);
#endif

  FName Section("FromBack");
  if (Theta >= -45.f && Theta < 45.f) {
    Section = FName("FromFront");
  } else if (Theta >= -135.f && Theta < -45.f) {
    Section = FName("FromLeft");
  } else if (Theta >= 45.f && Theta < 135.f) {
    Section = FName("FromRight");
  }
  PlayHitReactMontage(Section);
}

bool ABaseCharacter::CanAttack() {
  return false;
}

TArray<FName> ABaseCharacter::GetAllMontageSectionsNames(const UAnimMontage* const Montage) {
  TArray<FName> SectionNames;

  if (Montage != nullptr) {
    const int32 NumSections = Montage->GetNumSections();
    for (int32 i = 0; i < NumSections; ++i) {
      const FName SectionName = Montage->GetSectionName(i);
      if (SectionName != NAME_None) {
        SectionNames.Add(SectionName);
      }
    }
  }
  return SectionNames;
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint) {
  if (HitSound && GetWorld()) {
    UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
  }
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint) {
  if (HitParticles && GetWorld()) {
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ImpactPoint);
  }
}

void ABaseCharacter::HandleDamage(const float DamageAmount) {
  if (AttributeComponent) {
    AttributeComponent->ReceiveDamage(DamageAmount);
  }
}
