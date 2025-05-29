// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "DebugItem.h"
#include "../Common/DebugShape.h"

void ADebugItem::RenderDebugShape() const {
  static const FVector& CurrentLocation = GetActorLocation();

  DELTA_DEBUG_SPHERE_ONE_FRAME(CurrentLocation);
  DELTA_DEBUG_ARROW_ONE_FRAME(CurrentLocation,
                              CurrentLocation + GetActorForwardVector().GetSafeNormal() * 100.f);
}

void ADebugItem::RenderDebugShapeOneFrame(const float DeltaTime) {
  // Frequency * RunningTime
  //      (Hz) * (s)         = (1 / s) * (s) = 1 = scalar = unitless
  const float DeltaZ = GetSineOscillationOffset();

  // MovementRate * DeltaTime
  //       (cm/s) * (s/frame) = (cm/frame)
  AddActorWorldOffset(FVector(MovementRate * DeltaTime, 0.f, DeltaZ));

  // RotationRate * DeltaTime
  //  (degrees/s) * (s/frame) = (degrees/frame)
  AddActorWorldRotation(FRotator(0.f, RotationRate * DeltaTime, 0.f));

  RenderDebugShape();
}

void ADebugItem::BeginPlayAction() {
  SetLocation(FVector(0.f, 0.f, 50.f));
  SetRotation(FRotator(0.f, 45.f, 0.f));
  UpdateForwardDirection();
}

void ADebugItem::TickAction(const float DeltaTime) {
  RenderDebugShapeOneFrame(DeltaTime);
}