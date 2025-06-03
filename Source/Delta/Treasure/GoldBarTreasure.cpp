// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "GoldBarTreasure.h"
#include "../Common/Finder.h"

AGoldBarTreasure::AGoldBarTreasure() {
  static const TCHAR* const Path = TEXT("/Script/Engine.StaticMesh'/Game/AncientTreasures/Meshes/SM_GoldBar_01a.SM_GoldBar_01a'");
  DELTA_SET_STATIC_MESH(StaticMeshComponent, Path);

  SphereComponent->SetRelativeLocation(FVector(0, 0, 3.5f));
  SphereComponent->SetSphereRadius(13.0f);

  Gold = 7.0f;
}
