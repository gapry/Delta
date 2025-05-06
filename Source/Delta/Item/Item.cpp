// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Item.h"
#include "../Common/LogUtil.h"
#include "../Common/DebugShape.h"

AItem::AItem() {
  PrimaryActorTick.bCanEverTick = true;

  if (RootComponent) {
    RootComponent->SetMobility(EComponentMobility::Movable);
  }
}

void AItem::BeginPlay() {
  Super::BeginPlay();
}

void AItem::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
  RenderDebugShapeOneFrame(DeltaTime);
}

void AItem::RenderDebugShape() const {
  const FVector CurrentLocation = GetActorLocation();

  DELTA_DEBUG_SPHERE_ONE_FRAME(CurrentLocation);
  DELTA_DEBUG_ARROW_ONE_FRAME(CurrentLocation,
                              CurrentLocation + GetActorForwardVector().GetSafeNormal() * 100.f);
}

void AItem::RenderDebugShapeOneFrame(const float DeltaTime) {
  // MovementRate * DeltaTime
  //       (cm/s) * (s/frame) = (cm/frame)
  AddActorWorldOffset(FVector(MovementRate * DeltaTime, 0.f, 0.f));

  // RotationRate * DeltaTime
  //       (cm/s) * (s/frame) = (cm/frame)
  AddActorWorldRotation(FRotator(0.f, RotationRate * DeltaTime, 0.f));

  RenderDebugShape();
}

void AItem::SetLocation(const FVector& NewLocation) {
  SetActorLocation(NewLocation);
  DELTA_LOG("{}", DeltaFormat("CurrentLocation: {}", GetActorLocation().ToString()));
}

void AItem::SetRotation(const FRotator& NewRotation) {
  SetActorRotation(NewRotation);
  DELTA_LOG("{}", DeltaFormat("CurrentRotation: {}", GetActorRotation().ToString()));
}

void AItem::UpdateForwardDirection() {
  ForwardDirection = GetActorForwardVector().GetSafeNormal();
  DELTA_LOG("{}", DeltaFormat("ForwardDirection: {}", ForwardDirection.ToString()));
}
