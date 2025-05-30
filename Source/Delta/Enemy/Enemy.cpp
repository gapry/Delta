// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../Common/Finder.h"
#include "../Common/LogUtil.h"

AEnemy::AEnemy() {
  {
    PrimaryActorTick.bCanEverTick = true;
  }

  {
    SkeletalMeshComponent = GetMesh();

    static constexpr const TCHAR* const SkeletalMeshPath{TEXT(
      "/Script/Engine.SkeletalMesh'/Game/Mixamo/Paladin/Paladin_J_Nordstrom.Paladin_J_Nordstrom'")};

    DELTA_SET_SKELETAL_MESH(SkeletalMeshComponent.Get(), SkeletalMeshPath);

    SkeletalMeshComponent->SetRelativeTransform(FTransform(FRotator(0.f, -90.f, 0.f),
                                                           FVector(0.f, 0.f, -85.332687f),
                                                           FVector(1.f, 1.f, 1.f)));

    SkeletalMeshComponent->SetGenerateOverlapEvents(true);

    SkeletalMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,
                                                         ECollisionResponse::ECR_Block);
    SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,
                                                         ECollisionResponse::ECR_Ignore);
  }

  {
    CapsuleComponent = GetCapsuleComponent();

    CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,
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
}