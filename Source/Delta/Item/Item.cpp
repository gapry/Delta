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

  DELTA_LOG("{}", DELTA_FUNCSIG);

  const FVector CurrentLocation = GetActorLocation();
  const FVector CurrentForward  = GetActorForwardVector();

  DELTA_DEBUG_POINT(CurrentLocation);
  DELTA_DEBUG_LINE(CurrentLocation, CurrentLocation + CurrentForward * 100.f);
  DELTA_DEBUG_ARROW(CurrentLocation, CurrentLocation + CurrentForward * 150.f);
  DELTA_DEBUG_SPHERE(CurrentLocation);
}

void AItem::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  DELTA_LOG_ONCE("{}", DeltaFormat("DeltaTime = {}", DeltaTime));
}
