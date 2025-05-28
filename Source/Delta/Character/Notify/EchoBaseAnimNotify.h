// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EchoBaseAnimNotify.generated.h"

class AEchoCharacter;

UCLASS()
class DELTA_API UEchoBaseAnimNotify : public UAnimNotify {
  GENERATED_BODY()

public:
  virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
  virtual void ExecuteNotifyAction();

  UPROPERTY()
  TObjectPtr<AEchoCharacter> EchoCharacter{nullptr};
};
