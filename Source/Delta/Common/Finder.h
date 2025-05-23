// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "LogUtil.h"

#define DELTA_SET_STATIC_MESH(StaticMeshComponent, MeshPath)                                   \
  do {                                                                                         \
    if (!(StaticMeshComponent) || !(MeshPath)) {                                               \
      DELTA_LOG("{}", DeltaFormat("StaticMeshComponent is null or MeshPath is null"));         \
      break;                                                                                   \
    }                                                                                          \
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Finder(MeshPath);                    \
    if (!Finder.Succeeded()) {                                                                 \
      DELTA_LOG("{}", DeltaFormat("Failed to load static mesh: {}", TCHAR_TO_UTF8(MeshPath))); \
      break;                                                                                   \
    }                                                                                          \
    (StaticMeshComponent)->SetStaticMesh(Finder.Object);                                       \
  } while (false)
// ----

#define DELTA_SET_SKELETAL_MESH(SkeletalMeshComponent, MeshPath)                                 \
  do {                                                                                           \
    if (!(SkeletalMeshComponent) || !(MeshPath)) {                                               \
      DELTA_LOG("{}", DeltaFormat("SkeletalMeshComponent is null or MeshPath is null"));         \
      break;                                                                                     \
    }                                                                                            \
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> Finder(MeshPath);                    \
    if (!Finder.Succeeded()) {                                                                   \
      DELTA_LOG("{}", DeltaFormat("Failed to load skeletal mesh: {}", TCHAR_TO_UTF8(MeshPath))); \
      break;                                                                                     \
    }                                                                                            \
    (SkeletalMeshComponent)->SetSkeletalMesh(Finder.Object);                                     \
  } while (false)
// ----

#define DELTA_SET_ANIMATION(MeshComponent, AnimSequencePath)                                  \
  do {                                                                                        \
    if (!(MeshComponent) || !(AnimSequencePath)) {                                            \
      DELTA_LOG("{}", DeltaFormat("MeshComponent is null or AnimSequencePath is null"));      \
      break;                                                                                  \
    }                                                                                         \
    static ConstructorHelpers::FObjectFinder<UAnimationAsset> AnimSequence(AnimSequencePath); \
    if (!AnimSequence.Succeeded()) {                                                          \
      DELTA_LOG(                                                                              \
        "{}",                                                                                 \
        DeltaFormat("Failed to load animation asset: {}", TCHAR_TO_UTF8(AnimSequencePath)));  \
      break;                                                                                  \
    }                                                                                         \
    (MeshComponent)->SetAnimation(AnimSequence.Object);                                       \
    (MeshComponent)->AnimationData.AnimToPlay = AnimSequence.Object;                          \
  } while (false)
// ----

#define DELTA_SET_InputMappingContext(InputMappingContext, PATH)                               \
  do {                                                                                         \
    if (!(PATH)) {                                                                             \
      DELTA_LOG("{}", DeltaFormat("InputMappingContext is null or PATH is null"));             \
      break;                                                                                   \
    }                                                                                          \
    static ConstructorHelpers::FObjectFinder<UInputMappingContext> Finder(PATH);               \
    if (!Finder.Succeeded()) {                                                                 \
      DELTA_LOG("{}",                                                                          \
                DeltaFormat("Failed to load input mapping context: {}", TCHAR_TO_UTF8(PATH))); \
      break;                                                                                   \
    }                                                                                          \
    (InputMappingContext) = Finder.Object;                                                     \
  } while (false)

#define DELTA_SET_InputAction(InputAction, PATH)                                            \
  do {                                                                                      \
    if (!(PATH)) {                                                                          \
      DELTA_LOG("{}", DeltaFormat("InputAction is null or PATH is null"));                  \
      break;                                                                                \
    }                                                                                       \
    static ConstructorHelpers::FObjectFinder<UInputAction> Finder(PATH);                    \
    if (!Finder.Succeeded()) {                                                              \
      DELTA_LOG("{}", DeltaFormat("Failed to load input action: {}", TCHAR_TO_UTF8(PATH))); \
      break;                                                                                \
    }                                                                                       \
    (InputAction) = Finder.Object;                                                          \
  } while (false)
// ----

#define DELTA_SET_GROOM(GroomComponent, GroomPath)                                              \
  do {                                                                                          \
    if (!(GroomComponent) || !(GroomPath)) {                                                    \
      DELTA_LOG("{}", DeltaFormat("GroomComponent is null or GroomPath is null"));              \
      break;                                                                                    \
    }                                                                                           \
    static ConstructorHelpers::FObjectFinder<UGroomAsset> Finder(GroomPath);                    \
    if (!Finder.Succeeded()) {                                                                  \
      DELTA_LOG("{}", DeltaFormat("Failed to load groom asset: {}", TCHAR_TO_UTF8(GroomPath))); \
      break;                                                                                    \
    }                                                                                           \
    (GroomComponent)->SetGroomAsset(Finder.Object);                                             \
  } while (false)
// ----

#define DELTA_SET_GROOM_MATERIAL(HairComponent, MaterialIndex, MaterialPath)                    \
  do {                                                                                          \
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> Finder(MaterialPath);          \
    if (!Finder.Succeeded()) {                                                                  \
      DELTA_LOG("{}", DeltaFormat("Failed to load material: {}", TCHAR_TO_UTF8(MaterialPath))); \
    }                                                                                           \
    HairComponent->SetMaterial(MaterialIndex, Finder.Object);                                   \
  } while (false)
// ----