// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "QuadPyramid.h"
#include "../Common/Finder.h"

AQuadPyramid::AQuadPyramid() {
  static const TCHAR* const Path =
    TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_QuadPyramid'");
  Finder::SetStaticMesh(StaticMeshComponent, Path);
}

void AQuadPyramid::OnConstruction(const FTransform& Transform) {
  Super::OnConstruction(Transform);

  Super::InitializeCollision();
  Super::InitializeRootComponent();
}

void AQuadPyramid::BeginPlayAction() {
}

void AQuadPyramid::TickAction(const float DeltaTime) {
}