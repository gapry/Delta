// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "BaseTreasure.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/Echo/EchoCharacter.h"
#include "../Common/Finder.h"

ABaseTreasure::ABaseTreasure() {
  // Sound effect by Eric Matyas - www.soundimage.org
  static const TCHAR* const Path = TEXT("/Script/MetasoundEngine.MetaSoundSource'/Game/Delta/MetaSound/sfx_Treasure.sfx_Treasure'");
  DELTA_SET_SOUNDBASE(PickupSound, Path);

  StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
  StaticMeshComponent->UpdateCollisionProfile();
}

void ABaseTreasure::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                         AActor*              OtherActor,
                                         UPrimitiveComponent* OtherComp,
                                         int32                OtherBodyIndex,
                                         bool                 bFromSweep,
                                         const FHitResult&    SweepResult) {
  auto* const EchoCharacter = Cast<AEchoCharacter>(OtherActor);
  if (EchoCharacter) {
    if (PickupSound) {
      UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
    }
    Destroy();
  }
}

void ABaseTreasure::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  FRotator Rotation = GetActorRotation();
  Rotation.Yaw += 60.f * DeltaTime;
  SetActorRotation(Rotation);
}