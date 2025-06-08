// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "EnemyAnimInstance.h"
#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UEnemyAnimInstance::NativeInitializeAnimation() {
  Super::NativeInitializeAnimation();

  EnemyCharacter = Cast<AEnemy>(TryGetPawnOwner());
  if (EnemyCharacter) {
    CharacterMovement = EnemyCharacter->GetCharacterMovement();
  }
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaTime) {
  Super::NativeUpdateAnimation(DeltaTime);

  if (CharacterMovement) {
    GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovement->Velocity);
  }
}
