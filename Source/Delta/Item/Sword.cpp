// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Sword.h"
#include "Components/SphereComponent.h"
#include "../Common/Finder.h"
#include "../Character/EchoCharacter.h"

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

void ASword::BeginPlayAction() {
}

void ASword::TickAction(const float DeltaTime) {
  // const float DeltaZ = GetSineOscillationOffset();

  // FVector MoveLocation = GetActorLocation();
  // MoveLocation.Z += DeltaZ;
  // SetActorLocation(MoveLocation);

  // FRotator MoveRotation = GetActorRotation();
  // MoveRotation.Yaw += RotationRate * DeltaTime;
  // SetActorRotation(MoveRotation);
}

void ASword::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                  AActor*              OtherActor,
                                  UPrimitiveComponent* OtherComp,
                                  int32                OtherBodyIndex,
                                  bool                 bFromSweep,
                                  const FHitResult&    SweepResult) {
  AEchoCharacter* EchoCharacter = Cast<AEchoCharacter>(OtherActor);
  if (EchoCharacter) {
    EchoCharacter->SetOverlappingItem(this);
  }
}

void ASword::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                AActor*              OtherActor,
                                UPrimitiveComponent* OtherComp,
                                int32                OtherBodyIndex) {
  AEchoCharacter* EchoCharacter = Cast<AEchoCharacter>(OtherActor);
  if (EchoCharacter) {
    EchoCharacter->SetOverlappingItem(nullptr);
  }
}

void ASword::Equip(USceneComponent* InParent, FName InSocketName) {
  FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
  StaticMeshComponent->AttachToComponent(InParent, TransformRules, InSocketName);
}