// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "BaseBreakable.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "../Common/LogUtil.h"

ABaseBreakable::ABaseBreakable() {
  GeometryCollectionComponent =
    CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));

  if (!GeometryCollectionComponent) {
    DELTA_LOG("{}", DeltaFormat("Failed to create GeometryCollectionComponent"));
  }
  RootComponent = GeometryCollectionComponent;
}

void ABaseBreakable::BeginPlay() {
  Super::BeginPlay();
}

void ABaseBreakable::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
}