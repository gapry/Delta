// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "BaseBreakable.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Misc/AssertionMacros.h"
#include "../Common/LogUtil.h"

ABaseBreakable::ABaseBreakable() {
  {
    PrimaryActorTick.bCanEverTick = true;
  }

  {
    GeometryCollectionComponent =
      CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));

    check(GeometryCollectionComponent);
    DELTA_LOG("{}", DeltaFormat("GeometryCollectionComponent created successfully"));

    RootComponent = GeometryCollectionComponent;
  }
}

void ABaseBreakable::BeginPlay() {
  Super::BeginPlay();
}

void ABaseBreakable::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
}