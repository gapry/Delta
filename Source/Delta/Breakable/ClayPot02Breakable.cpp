// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "ClayPot02Breakable.h"

AClayPot02Breakable::AClayPot02Breakable() {
  static const TCHAR* const Path =
    TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/Delta/Destructible/GC_SM_ClayPot_02.GC_SM_ClayPot_02'");
  DELTA_SET_GEOMETRY_COLLECTION(GeometryCollectionComponent, Path);

  Capsule->SetRelativeLocation(FVector(0, 0, 32.0));
  Capsule->SetCapsuleHalfHeight(52.0);
  Capsule->SetCapsuleRadius(32.0);
}
