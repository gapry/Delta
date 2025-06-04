// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "PotCCompleteBreakable.h"

APotCCompleteBreakable::APotCCompleteBreakable() {
  static const TCHAR* const Path = TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/Delta/Destructible/"
                                        "GC_SM_Pot_C_Complete.GC_SM_Pot_C_Complete'");
  DELTA_SET_GEOMETRY_COLLECTION(GeometryCollectionComponent, Path);

  Capsule->SetRelativeLocation(FVector(0, 0, 16.0));
  Capsule->SetCapsuleHalfHeight(25.0);
  Capsule->SetCapsuleRadius(15.0);
}
