// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Gems01bTreasure.h"
#include "../Common/Finder.h"

AGems01bTreasure::AGems01bTreasure() {
  static const TCHAR* const Path = TEXT("/Script/Engine.StaticMesh'/Game/AncientTreasures/Meshes/SM_Gems_01b.SM_Gems_01b'");
  DELTA_SET_STATIC_MESH(StaticMeshComponent, Path);

  SphereComponent->SetRelativeLocation(FVector(0, 0, 2.8f));
  SphereComponent->SetSphereRadius(7.5f);

  Gold = 15.0f;
}
