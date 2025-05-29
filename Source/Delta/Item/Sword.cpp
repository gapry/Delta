// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Sword.h"
#include "Components/SphereComponent.h"
#include "../Common/Finder.h"

ASword::ASword() {
  {
    static const TCHAR* const Path =
      TEXT("/Script/Engine.StaticMesh'/Game/Fab/Megascans/3D/Sword_uitlbiaga/Medium/"
           "SM_uitlbiaga_tier_2.SM_uitlbiaga_tier_2'");
    DELTA_SET_STATIC_MESH(StaticMeshComponent, Path);
  }

  {
    SphereComponent->SetSphereRadius(52.f);
  }

  {
    StaticMeshComponent->SetCollisionProfileName(TEXT("Custom"));
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    StaticMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,
                                                       ECollisionResponse::ECR_Ignore);
    StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,
                                                       ECollisionResponse::ECR_Overlap);
    StaticMeshComponent->UpdateCollisionProfile();
  }
}