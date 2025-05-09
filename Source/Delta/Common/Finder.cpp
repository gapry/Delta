// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Finder.h"
#include "../Common/LogUtil.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

void Finder::InitializeStaticMeshComponent(UStaticMeshComponent* StaticMeshComponent,
                                           const TCHAR*          MeshPath) {
  if (StaticMeshComponent != nullptr && MeshPath != nullptr) {
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Finder(MeshPath);
    if (Finder.Succeeded()) {
      StaticMeshComponent->SetStaticMesh(Finder.Object);
    } else {
      DELTA_LOG("{}", DeltaFormat("Failed to load static mesh: {}", TCHAR_TO_UTF8(MeshPath)));
    }
  } else {
    DELTA_LOG("{}", DeltaFormat("StaticMeshComponent is null or MeshPath is null"));
  }
}

void Finder::InitializeSkeletalMeshComponent(USkeletalMeshComponent* SkeletalMeshComponent,
                                             const TCHAR*            MeshPath) {
  if (SkeletalMeshComponent != nullptr && MeshPath != nullptr) {
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> Finder(MeshPath);
    if (Finder.Succeeded()) {
      SkeletalMeshComponent->SetSkeletalMesh(Finder.Object);
    } else {
      DELTA_LOG("{}", DeltaFormat("Failed to load skeletal mesh: {}", TCHAR_TO_UTF8(MeshPath)));
    }
  } else {
    DELTA_LOG("{}", DeltaFormat("SkeletalMeshComponent is null or MeshPath is null"));
  }
}