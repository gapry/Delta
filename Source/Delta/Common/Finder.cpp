// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Finder.h"
#include "../Common/LogUtil.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"

void Finder::SetStaticMesh(UStaticMeshComponent* const StaticMeshComponent,
                           const TCHAR* const          MeshPath) {
  if (!StaticMeshComponent || !MeshPath) {
    DELTA_LOG("{}", DeltaFormat("StaticMeshComponent is null or MeshPath is null"));
    return;
  }

  static ConstructorHelpers::FObjectFinder<UStaticMesh> Finder(MeshPath);
  if (!Finder.Succeeded()) {
    DELTA_LOG("{}", DeltaFormat("Failed to load static mesh: {}", TCHAR_TO_UTF8(MeshPath)));
    return;
  }

  StaticMeshComponent->SetStaticMesh(Finder.Object);
}

void Finder::SetSkeletalMesh(USkeletalMeshComponent* const SkeletalMeshComponent,
                             const TCHAR* const            MeshPath) {
  if (!SkeletalMeshComponent || !MeshPath) {
    DELTA_LOG("{}", DeltaFormat("SkeletalMeshComponent is null or MeshPath is null"));
    return;
  }

  ConstructorHelpers::FObjectFinder<USkeletalMesh> Finder(MeshPath);
  if (!Finder.Succeeded()) {
    DELTA_LOG("{}", DeltaFormat("Failed to load skeletal mesh: {}", TCHAR_TO_UTF8(MeshPath)));
    return;
  }

  SkeletalMeshComponent->SetSkeletalMesh(Finder.Object);
}

void Finder::SetAnimation(USkeletalMeshComponent* const MeshComponent,
                          const TCHAR* const            AnimSequencePath) {
  if (!MeshComponent || !AnimSequencePath) {
    DELTA_LOG("{}", DeltaFormat("MeshComponent is null or AnimSequencePath is null"));
    return;
  }

  static ConstructorHelpers::FObjectFinder<UAnimationAsset> AnimSequence(AnimSequencePath);
  if (!AnimSequence.Succeeded()) {
    DELTA_LOG("{}",
              DeltaFormat("Failed to load animation asset: {}", TCHAR_TO_UTF8(AnimSequencePath)));
    return;
  }

  MeshComponent->SetAnimation(AnimSequence.Object);
  MeshComponent->AnimationData.AnimToPlay = AnimSequence.Object;
}

UInputMappingContext* Finder::FindInputMappingContext(const TCHAR* const Path) {
  if (!Path) {
    DELTA_LOG("{}", DeltaFormat("Path is null"));
    return nullptr;
  }

  static ConstructorHelpers::FObjectFinder<UInputMappingContext> Finder(Path);
  if (!Finder.Succeeded()) {
    DELTA_LOG("{}", DeltaFormat("Failed to load input mapping context: {}", TCHAR_TO_UTF8(Path)));
    return nullptr;
  }
  return Finder.Object;
}

UInputAction* Finder::FindInputAction(const TCHAR* const Path) {
  if (!Path) {
    DELTA_LOG("{}", DeltaFormat("Path is null"));
    return nullptr;
  }

  ConstructorHelpers::FObjectFinder<UInputAction> Finder(Path);
  if (!Finder.Succeeded()) {
    DELTA_LOG("{}", DeltaFormat("Failed to load input action: {}", TCHAR_TO_UTF8(Path)));
    return nullptr;
  }
  return Finder.Object;
}
