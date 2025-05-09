// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Bird.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "../Common/LogUtil.h"

ABird::ABird() {
  PrimaryActorTick.bCanEverTick = true;

  static constexpr const TCHAR* const ComponentName = TEXT("SkeletalMeshComponent");
  SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(ComponentName);

  static constexpr const TCHAR* const CapsuleName = TEXT("CapsuleComponent");
  CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(CapsuleName);

  if (CapsuleComponent) {
    RootComponent = CapsuleComponent;
  }

  static const TCHAR* const SkeletalMeshPath =
    TEXT("SkeletalMesh'/Game/AnimalVarietyPack/Crow/Meshes/SK_Crow.SK_Crow'");
  InitializeSkeletalMeshComponent(SkeletalMeshPath);
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

void ABird::InitializeSkeletalMeshComponent(const TCHAR* const SkeletalMeshPath) {
  if (SkeletalMeshComponent != nullptr && SkeletalMeshPath != nullptr) {
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> Finder(SkeletalMeshPath);
    if (Finder.Succeeded()) {
      SkeletalMeshComponent->SetSkeletalMesh(Finder.Object);
    } else {
      DELTA_LOG("{}",
                DeltaFormat("Failed to load skeletal mesh: {}", TCHAR_TO_UTF8(SkeletalMeshPath)));
    }
  }
}

void ABird::PostInitializeComponents() {
  Super::PostInitializeComponents();

  PostInitializeSkeletalMeshComponent();
  InitializeCapsuleComponent();
  InitializeCollision();
}

void ABird::PostInitializeSkeletalMeshComponent() {
  if (SkeletalMeshComponent) {
    SkeletalMeshComponent->SetMobility(EComponentMobility::Movable);

    SkeletalMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -10.f));
    SkeletalMeshComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
  }
}

void ABird::InitializeCapsuleComponent() {
  if (CapsuleComponent) {
    CapsuleComponent->SetCapsuleHalfHeight(20.f);
    CapsuleComponent->SetCapsuleRadius(15.f);
    CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  }
}

void ABird::InitializeCollision() {
  static constexpr const TCHAR* const CollisionProfileName = TEXT("BlockAll");

  if (SkeletalMeshComponent) {
    SkeletalMeshComponent->SetCollisionProfileName(CollisionProfileName);
    SkeletalMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
    SkeletalMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
  }
}