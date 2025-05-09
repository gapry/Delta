// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

class UStaticMeshComponent;
class USkeletalMeshComponent;

class DELTA_API Finder {
public:
  static void InitializeStaticMeshComponent(UStaticMeshComponent* StaticMeshComponent,
                                            const TCHAR*          MeshPath);

  static void InitializeSkeletalMeshComponent(USkeletalMeshComponent* SkeletalMeshComponent,
                                              const TCHAR*            MeshPath);

  static void InitializeAnimationAsset(USkeletalMeshComponent* MeshComponent,
                                       const TCHAR*            AnimSequencePath);
};