// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "EchoAnimInstance.generated.h"

class AEchoCharacter;
class UCharacterMovementComponent;

UCLASS()
class DELTA_API UEchoAnimInstance : public UAnimInstance {
  GENERATED_BODY()

public:
  virtual void NativeInitializeAnimation() override;
  virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
  UPROPERTY(BlueprintReadOnly, Category = "EchoAnimInstance", meta = (AllowPrivateAccess = "true"))
  TSoftObjectPtr<AEchoCharacter> EchoCharacter;

  UPROPERTY(BlueprintReadOnly, Category = "EchoAnimInstance", meta = (AllowPrivateAccess = "true"))
  TSoftObjectPtr<UCharacterMovementComponent> CharacterMovement;

  UPROPERTY(BlueprintReadOnly, Category = "EchoAnimInstance", meta = (AllowPrivateAccess = "true"))
  float GroundSpeed{0.0f};

  UPROPERTY(BlueprintReadOnly, Category = "EchoAnimInstance", meta = (AllowPrivateAccess = "true"))
  bool IsFalling{false};

  UPROPERTY(BlueprintReadOnly,
            Category = "EchoAnimInstance | CharacterState",
            meta     = (AllowPrivateAccess = "true"))
  ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
};
