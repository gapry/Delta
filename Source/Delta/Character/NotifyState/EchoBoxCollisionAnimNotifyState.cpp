// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "EchoBoxCollisionAnimNotifyState.h"
#include "../EchoCharacter.h"

void UEchoBoxCollisionAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp,
                                                   UAnimSequenceBase*      Animation,
                                                   float                   TotalDuration) {
  SetWeaponCollisionEnabled(MeshComp, ECollisionEnabled::QueryOnly);
}

void UEchoBoxCollisionAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp,
                                                 UAnimSequenceBase*      Animation) {
  SetWeaponCollisionEnabled(MeshComp, ECollisionEnabled::NoCollision);
}

void UEchoBoxCollisionAnimNotifyState::SetWeaponCollisionEnabled(
  USkeletalMeshComponent* MeshComp,
  ECollisionEnabled::Type CollisionEnabled) {
  if (MeshComp && MeshComp->GetOwner()) {
    EchoCharacter = Cast<AEchoCharacter>(MeshComp->GetOwner());
    if (EchoCharacter) {
      EchoCharacter->SetWeaponCollisionEnabled(CollisionEnabled);
    }
  }
}