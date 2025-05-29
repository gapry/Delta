// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "EchoFinishEquippingAnimNotify.h"
#include "../EchoCharacter.h"

void UEchoFinishEquippingAnimNotify::ExecuteNotifyAction() {
  Super::ExecuteNotifyAction();

  if (EchoCharacter) {
    EchoCharacter->FinishEquipping();
  }
}