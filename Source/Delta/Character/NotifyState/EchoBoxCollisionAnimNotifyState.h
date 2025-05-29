// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "EchoBoxCollisionAnimNotifyState.generated.h"

class AEchoCharacter;

UCLASS()
class DELTA_API UEchoBoxCollisionAnimNotifyState : public UAnimNotifyState {
  GENERATED_BODY()

public:
  virtual void NotifyBegin(USkeletalMeshComponent* MeshComp,
                           UAnimSequenceBase*      Animation,
                           float                   TotalDuration) override;

  virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
  void SetWeaponCollisionEnabled(USkeletalMeshComponent* MeshComp,
                                 ECollisionEnabled::Type CollisionEnabled);

  UPROPERTY()
  TObjectPtr<AEchoCharacter> EchoCharacter{nullptr};
};
