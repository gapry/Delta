// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

class UStaticMeshComponent;
class USkeletalMeshComponent;
class UInputMappingContext;
class UInputAction;

class DELTA_API Finder {
public:
  static void SetStaticMesh(UStaticMeshComponent* const StaticMeshComponent,
                            const TCHAR* const          MeshPath);

  static void SetSkeletalMesh(USkeletalMeshComponent* const SkeletalMeshComponent,
                              const TCHAR* const            MeshPath);

  static void SetAnimation(USkeletalMeshComponent* const MeshComponent,
                           const TCHAR* const            AnimSequencePath);

  static UInputMappingContext* FindInputMappingContext(const TCHAR* const Path);
};