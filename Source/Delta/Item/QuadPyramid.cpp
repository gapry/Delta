// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "QuadPyramid.h"

AQuadPyramid::AQuadPyramid() {
  static const TCHAR* const Path =
    TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_QuadPyramid'");
  InitializeStaticMeshComponent(Path);
}

void AQuadPyramid::OnConstruction(const FTransform& Transform) {
  Super::OnConstruction(Transform);

  InitializeCollision();
  InitializeRootComponent();
}

void AQuadPyramid::InitializeCollision() {
  Super::InitializeCollision();
}

void AQuadPyramid::InitializeRootComponent() {
  Super::InitializeRootComponent();
};

void AQuadPyramid::BeginPlay() {
  Super::BeginPlay();
  BeginPlayAction();
}

void AQuadPyramid::BeginPlayAction() {
}

void AQuadPyramid::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
  TickAction(DeltaTime);
}

void AQuadPyramid::TickAction(const float DeltaTime) {
}