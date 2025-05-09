// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Bird.h"
#include "../Common/Finder.h"
#include "../Common/LogUtil.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

ABird::ABird() {
  PrimaryActorTick.bCanEverTick = true;

  {
    static constexpr const TCHAR* const ComponentName = TEXT("SkeletalMeshComponent");
    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(ComponentName);

    static const TCHAR* const MeshPath =
      TEXT("SkeletalMesh'/Game/AnimalVarietyPack/Crow/Meshes/SK_Crow.SK_Crow'");
    Finder::InitializeSkeletalMeshComponent(SkeletalMeshComponent, MeshPath);

    static constexpr const TCHAR* const AnimSequencePath =
      TEXT("AnimSequence'/Game/AnimalVarietyPack/Crow/Animations/ANIM_Crow_Fly.ANIM_Crow_Fly'");
    Finder::InitializeAnimationAsset(SkeletalMeshComponent, AnimSequencePath);
  }

  {
    static constexpr const TCHAR* const CapsuleName = TEXT("CapsuleComponent");
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(CapsuleName);
  }

  {
    if (CapsuleComponent) {
      RootComponent = CapsuleComponent;

      if (SkeletalMeshComponent) {
        SkeletalMeshComponent->AttachToComponent(CapsuleComponent,
                                                 FAttachmentTransformRules::KeepRelativeTransform);
      } else {
        DELTA_LOG("{}", DeltaFormat("SkeletalMeshComponent is null"));
      }
    } else {
      DELTA_LOG("{}", DeltaFormat("CapsuleComponent is null"));
    }
  }
}

void ABird::BeginPlay() {
  Super::BeginPlay();
}

void ABird::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
}

void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABird::PostInitializeComponents() {
  Super::PostInitializeComponents();

  PostInitializeSkeletalMeshComponent();
  InitializeCapsuleComponent();
  InitializeCollision();
}

void ABird::PostInitializeSkeletalMeshComponent() {
  if (!SkeletalMeshComponent) {
    DELTA_LOG("{}", DeltaFormat("SkeletalMeshComponent is null"));
    return;
  }

  SkeletalMeshComponent->SetMobility(EComponentMobility::Movable);

  SkeletalMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -10.f));
  SkeletalMeshComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

  SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationSingleNode);
  SkeletalMeshComponent->AnimationData.bSavedPlaying = true;
  SkeletalMeshComponent->AnimationData.bSavedLooping = true;
}

void ABird::InitializeCapsuleComponent() {
  if (!CapsuleComponent) {
    DELTA_LOG("{}", DeltaFormat("CapsuleComponent is null"));
    return;
  }

  CapsuleComponent->SetCapsuleHalfHeight(20.f);
  CapsuleComponent->SetCapsuleRadius(15.f);
}

void ABird::InitializeCollision() {
  if (!CapsuleComponent) {
    DELTA_LOG("{}", DeltaFormat("CapsuleComponent is null"));
    return;
  }

  static constexpr const TCHAR* const CollisionProfileName = TEXT("BlockAll");

  CapsuleComponent->SetCollisionProfileName(CollisionProfileName);
  CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
  CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
  CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Block);
  CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}