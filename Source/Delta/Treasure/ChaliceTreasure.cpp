// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "ChaliceTreasure.h"
#include "../Common/Finder.h"

AChaliceTreasure::AChaliceTreasure() {
  static const TCHAR* const Path = TEXT("/Script/Engine.StaticMesh'/Game/AncientTreasures/Meshes/SM_Chalice_01a.SM_Chalice_01a'");
  DELTA_SET_STATIC_MESH(StaticMeshComponent, Path);
}