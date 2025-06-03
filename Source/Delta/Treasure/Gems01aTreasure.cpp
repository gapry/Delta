// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Gems01aTreasure.h"
#include "../Common/Finder.h"

AGems01aTreasure::AGems01aTreasure() {
  static const TCHAR* const Path = TEXT("/Script/Engine.StaticMesh'/Game/AncientTreasures/Meshes/SM_Gems_01a.SM_Gems_01a'");
  DELTA_SET_STATIC_MESH(StaticMeshComponent, Path);

  SphereComponent->SetRelativeLocation(FVector(0, 0, 4.0f));
  SphereComponent->SetSphereRadius(5.0f);

  Gold = 20.0f;
}
