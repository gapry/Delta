// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "EchoBaseAnimNotify.h"
#include "../EchoCharacter.h"

void UEchoBaseAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
  if (MeshComp && MeshComp->GetOwner()) {
    EchoCharacter = Cast<AEchoCharacter>(MeshComp->GetOwner());
    if (EchoCharacter) {
      ExecuteNotifyAction();
    }
  }
}

void UEchoBaseAnimNotify::ExecuteNotifyAction() {
}