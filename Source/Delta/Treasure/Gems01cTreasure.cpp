// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Gems01cTreasure.h"
#include "../Common/Finder.h"

AGems01cTreasure::AGems01cTreasure() {
  static const TCHAR* const Path = TEXT("/Script/Engine.StaticMesh'/Game/AncientTreasures/Meshes/SM_Gems_01c.SM_Gems_01c'");
  DELTA_SET_STATIC_MESH(StaticMeshComponent, Path);

  SphereComponent->SetRelativeLocation(FVector(0, 0, 1.5f));
  SphereComponent->SetSphereRadius(7.0f);

  Gold = 35.0f;
}
