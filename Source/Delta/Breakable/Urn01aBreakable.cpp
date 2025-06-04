// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Urn01aBreakable.h"

AUrn01aBreakable::AUrn01aBreakable() {
  static const TCHAR* const Path = TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/Delta/Destructible/"
                                        "GC_SM_Urn_01a.GC_SM_Urn_01a'");
  DELTA_SET_GEOMETRY_COLLECTION(GeometryCollectionComponent, Path);

  Capsule->SetRelativeLocation(FVector(0, 0, 20.0));
  Capsule->SetCapsuleHalfHeight(35.0);
  Capsule->SetCapsuleRadius(20.0);
}
