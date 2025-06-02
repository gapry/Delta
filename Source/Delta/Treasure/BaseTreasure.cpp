// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "BaseTreasure.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/EchoCharacter.h"

ABaseTreasure::ABaseTreasure() {
  // Sound effect by Eric Matyas - www.soundimage.org
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