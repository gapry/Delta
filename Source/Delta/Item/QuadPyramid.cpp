// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "QuadPyramid.h"

#include "../Common/Finder.h"

AQuadPyramid::AQuadPyramid() {
  static const TCHAR* const Path =
    TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_QuadPyramid'");
  DELTA_SET_STATIC_MESH(StaticMeshComponent, Path);
}

void AQuadPyramid::BeginPlayAction() {
}

void AQuadPyramid::TickAction(const float DeltaTime) {
}