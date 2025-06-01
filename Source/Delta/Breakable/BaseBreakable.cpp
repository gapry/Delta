// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "BaseBreakable.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Misc/AssertionMacros.h"
#include "../Common/LogUtil.h"

ABaseBreakable::ABaseBreakable() {
  PrimaryActorTick.bCanEverTick = true;

  GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
  check(GeometryCollectionComponent);

  GeometryCollectionComponent->bUseSizeSpecificDamageThreshold = false;
  GeometryCollectionComponent->SetGenerateOverlapEvents(true);
  GeometryCollectionComponent->SetNotifyRigidBodyCollision(false);

  GeometryCollectionComponent->SetCollisionProfileName(TEXT("Custom"));
  GeometryCollectionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
  GeometryCollectionComponent->SetCollisionObjectType(ECollisionChannel::ECC_Destructible);
  GeometryCollectionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  GeometryCollectionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
  GeometryCollectionComponent->UpdateCollisionProfile();

  RootComponent = GeometryCollectionComponent;
}

void ABaseBreakable::BeginPlay() {
  Super::BeginPlay();
}

void ABaseBreakable::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
}