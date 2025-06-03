// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "BangleTreasure.h"
#include "../Common/Finder.h"

ABangleTreasure::ABangleTreasure() {
  static const TCHAR* const Path = TEXT("/Script/Engine.StaticMesh'/Game/AncientTreasures/Meshes/SM_Bangle_01a.SM_Bangle_01a'");
  DELTA_SET_STATIC_MESH(StaticMeshComponent, Path);

  SphereComponent->SetRelativeLocation(FVector(0, 0, 5.0));
  SphereComponent->SetSphereRadius(16.0f);

  Gold = 65.0f;
}
