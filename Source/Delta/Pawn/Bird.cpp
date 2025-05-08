// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Bird.h"
#include "Components/SkeletalMeshComponent.h"
#include "../Common/LogUtil.h"

ABird::ABird() {
  PrimaryActorTick.bCanEverTick = true;

  static const TCHAR* const ComponentName = TEXT("SkeletalMeshComponent");
  SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(ComponentName);

  if (SkeletalMeshComponent) {
    RootComponent = SkeletalMeshComponent;
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
  InitializeCollision();
}

void ABird::PostInitializeSkeletalMeshComponent() {
  if (SkeletalMeshComponent) {
    SkeletalMeshComponent->SetMobility(EComponentMobility::Movable);
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