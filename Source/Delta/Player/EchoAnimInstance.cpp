// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "EchoAnimInstance.h"
#include "EchoCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UEchoAnimInstance::NativeInitializeAnimation() {
  Super::NativeInitializeAnimation();

  EchoCharacter = Cast<AEchoCharacter>(TryGetPawnOwner());
  if (EchoCharacter) {
    CharacterMovement = EchoCharacter->GetCharacterMovement();
  }
}

void UEchoAnimInstance::NativeUpdateAnimation(float DeltaTime) {
  Super::NativeUpdateAnimation(DeltaTime);

  if (CharacterMovement) {
    GroundSpeed    = UKismetMathLibrary::VSizeXY(CharacterMovement->Velocity);
    IsFalling      = CharacterMovement->IsFalling();
    CharacterState = EchoCharacter->GetCharacterState();
  }
}