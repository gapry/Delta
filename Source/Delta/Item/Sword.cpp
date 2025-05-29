// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Sword.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../Common/Finder.h"
#include "../Common/LogUtil.h"

ASword::ASword() {
  {
    static const TCHAR* const Path =
      TEXT("/Script/Engine.StaticMesh'/Game/Fab/Megascans/3D/Sword_uitlbiaga/Medium/"
           "SM_uitlbiaga_tier_2.SM_uitlbiaga_tier_2'");
    DELTA_SET_STATIC_MESH(StaticMeshComponent, Path);
  }

  {
    static const TCHAR* const Path =
      TEXT("/Script/MetasoundEngine.MetaSoundSource'/Game/Delta/MetaSound/sfx_Shink.sfx_Shink'");
    DELTA_SET_SOUNDBASE(EquipSound, Path);
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

void ASword::Equip(USceneComponent* InParent, FName InSocketName) {
  Super::Equip(InParent, InSocketName);

  if (EquipSound == nullptr) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "EquipSound is null"));
    return;
  }

  if (SphereComponent == nullptr) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "SphereComponent is null"));
    return;
  }

  SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
}