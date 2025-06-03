// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "EchoArmAnimNotify.h"
#include "../EchoCharacter.h"

void UEchoArmAnimNotify::ExecuteNotifyAction() {
  Super::ExecuteNotifyAction();

  if (EchoCharacter) {
    EchoCharacter->Arm();
  }
}