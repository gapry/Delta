// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "StatueTreasure.h"
#include "../Common/Finder.h"

AStatueTreasure::AStatueTreasure() {
  static const TCHAR* const Path = TEXT("/Script/Engine.StaticMesh'/Game/AncientTreasures/Meshes/SM_Statue_01a.SM_Statue_01a'");
  DELTA_SET_STATIC_MESH(StaticMeshComponent, Path);

  SphereComponent->SetRelativeLocation(FVector(0, 0, 48.0f));
  SphereComponent->SetSphereRadius(62.0f);

  Gold = 85.0f;
}
