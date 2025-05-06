// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Item.h"
#include "../Common/LogUtil.h"
#include "../Common/DebugShape.h"

AItem::AItem() {
  PrimaryActorTick.bCanEverTick = true;
}

void AItem::BeginPlay() {
  Super::BeginPlay();

  CurrentLocation = FVector(0.f, 0.f, 50.f);
  SetLocation(CurrentLocation);

  ForwardDirection = GetActorForwardVector();
  DELTA_DEBUG_ARROW(CurrentLocation, ForwardDirection * 100.f);
  DELTA_DEBUG_SPHERE(CurrentLocation);
}

void AItem::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
}

void AItem::SetLocation(const FVector& NewLocation) {
  SetActorLocation(NewLocation);
}