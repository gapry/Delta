// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "ClayPot01Breakable.h"

AClayPot01Breakable::AClayPot01Breakable() {
  static const TCHAR* const Path =
    TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/Delta/Destructible/GC_SM_ClayPot_01.GC_SM_ClayPot_01'");
  DELTA_SET_GEOMETRY_COLLECTION(GeometryCollectionComponent, Path);

  Capsule->SetRelativeLocation(FVector(0, 0, 28.0));
  Capsule->SetCapsuleHalfHeight(57.0);
  Capsule->SetCapsuleRadius(28.0);
}
