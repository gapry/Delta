// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Urn04aBreakable.h"

AUrn04aBreakable::AUrn04aBreakable() {
  static const TCHAR* const Path = TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/Delta/Destructible/"
                                        "GC_SM_Urn_04a.GC_SM_Urn_04a'");
  DELTA_SET_GEOMETRY_COLLECTION(GeometryCollectionComponent, Path);

  Capsule->SetRelativeLocation(FVector(0, 0, 60.0));
  Capsule->SetCapsuleHalfHeight(75.0);
  Capsule->SetCapsuleRadius(13.0);
}
