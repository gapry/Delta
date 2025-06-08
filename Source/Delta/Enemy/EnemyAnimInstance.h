// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

class AEnemy;
class UCharacterMovementComponent;

UCLASS()
class DELTA_API UEnemyAnimInstance : public UAnimInstance {
  GENERATED_BODY()

public:
  virtual void NativeInitializeAnimation() override;
  virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
  UPROPERTY(BlueprintReadOnly, Category = "EnemyAnimInstance", meta = (AllowPrivateAccess = "true"))
  TSoftObjectPtr<AEnemy> EnemyCharacter;

  UPROPERTY(BlueprintReadOnly, Category = "EnemyAnimInstance", meta = (AllowPrivateAccess = "true"))
  TSoftObjectPtr<UCharacterMovementComponent> CharacterMovement;

  UPROPERTY(BlueprintReadOnly, Category = "EnemyAnimInstance", meta = (AllowPrivateAccess = "true"))
  float GroundSpeed{0.0f};
};
