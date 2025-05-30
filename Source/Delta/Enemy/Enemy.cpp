// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Common/Finder.h"
#include "../Common/LogUtil.h"
#include "../Common/DebugShape.h"

AEnemy::AEnemy() {
  {
    PrimaryActorTick.bCanEverTick = true;
  }

  {
    SkeletalMeshComponent = GetMesh();

    static constexpr const TCHAR* const SkeletalMeshPath{
      TEXT("/Script/Engine.SkeletalMesh'/Game/Mixamo/Paladin/"
           "Sword_And_Shield_Idle.Sword_And_Shield_Idle'")};

    DELTA_SET_SKELETAL_MESH(SkeletalMeshComponent.Get(), SkeletalMeshPath);

    SkeletalMeshComponent->SetRelativeTransform(
      FTransform(FRotator(0.f, -90.f, 0.f), FVector(0.f, 0.f, -88.0f), FVector(1.f, 1.f, 1.f)));

    SkeletalMeshComponent->SetGenerateOverlapEvents(true);

    SkeletalMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,
                                                         ECollisionResponse::ECR_Block);
    SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,
                                                         ECollisionResponse::ECR_Ignore);
  }

  {
    static constexpr const TCHAR* const AnimBlueprintPath{TEXT(
      "/Script/Engine.AnimBlueprint'/Game/Delta/Enemy/Animation/Blueprint/ABP_Enemy.ABP_Enemy_C'")};
    DELTA_SET_ANIMATION_BLUEPRINT(SkeletalMeshComponent.Get(), AnimBlueprintPath);
  }

  {
    static constexpr const TCHAR* const MontagePath{TEXT(
      "/Script/Engine.AnimMontage'/Game/Delta/Enemy/Animation/Montage/AM_HitReact.AM_HitReact'")};
    DELTA_SET_ANIMATION_MONTAGE(HitReactMontage, MontagePath);
  }

  {
    CapsuleComponent = GetCapsuleComponent();

    CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,
                                                    ECollisionResponse::ECR_Ignore);
    CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,
                                                    ECollisionResponse::ECR_Ignore);
  }
}

void AEnemy::BeginPlay() {
  Super::BeginPlay();
}

void AEnemy::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::GetHit(const FVector& ImpactPoint) {
#if DELTA_DEBUG_HIT_RENDER
  DELTA_DEBUG_SPHERE_COLOR(ImpactPoint, FColor::Orange);
#endif
  DirectionalHitReact(ImpactPoint);
}

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint) {
  const FVector Forward = GetActorForwardVector();

  const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
  const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

  const double CosTheta = FVector::DotProduct(Forward, ToHit);
  double       Theta    = FMath::RadiansToDegrees(FMath::Acos(CosTheta));

  const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
  if (CrossProduct.Z < 0) {
    Theta *= -1.f;
  }

#if DELTA_DEBUG_HIT_RENDER
  UKismetSystemLibrary::DrawDebugArrow(this,
                                       GetActorLocation(),
                                       GetActorLocation() + Forward * 60.f,
                                       5.f,
                                       FColor::Red,
                                       5.f);

  UKismetSystemLibrary::DrawDebugArrow(this,
                                       GetActorLocation(),
                                       GetActorLocation() + ToHit * 60.f,
                                       5.f,
                                       FColor::Green,
                                       5.f);

  UKismetSystemLibrary::DrawDebugArrow(this,
                                       GetActorLocation(),
                                       GetActorLocation() + CrossProduct * 100.f,
                                       5.f,
                                       FColor::Blue,
                                       5.f);
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

void AEnemy::PlayHitReactMontage(const FName& SectionName) {
  UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
  if (AnimInstance && HitReactMontage) {
    AnimInstance->Montage_Play(HitReactMontage);
    AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
  }
}