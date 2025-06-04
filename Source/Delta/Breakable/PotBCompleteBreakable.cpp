// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "PotBCompleteBreakable.h"

APotBCompleteBreakable::APotBCompleteBreakable() {
  static const TCHAR* const Path = TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/Delta/Destructible/"
                                        "GC_SM_Pot_B_Complete.GC_SM_Pot_B_Complete'");
  DELTA_SET_GEOMETRY_COLLECTION(GeometryCollectionComponent, Path);

  Capsule->SetRelativeLocation(FVector(0, 0, 10.0));
  Capsule->SetCapsuleHalfHeight(20.0);
  Capsule->SetCapsuleRadius(20.0);
}
