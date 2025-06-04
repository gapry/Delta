// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "GargoyleStatueStand.h"
#include "Components/SphereComponent.h"
#include "../Common/Finder.h"

AGargoyleStatueStand::AGargoyleStatueStand() {
  {
    static const TCHAR* const Path =
      TEXT("/Script/Engine.StaticMesh'/Game/MedievalDungeon/Meshes/Props/SM_Gargoyle_Statue_Stand.SM_Gargoyle_Statue_Stand'");
    DELTA_SET_STATIC_MESH(StaticMeshComponent, Path);
  }

  {
    SphereComponent->SetSphereRadius(63.f);
    SphereComponent->SetRelativeLocation(FVector(0, 0, 36.0f));
    SphereComponent->SetCollisionProfileName(TEXT("Custom"));
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SphereComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
    SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
  }

  {
    StaticMeshComponent->SetCollisionProfileName(TEXT("Custom"));
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    StaticMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
    StaticMeshComponent->UpdateCollisionProfile();
  }
}
