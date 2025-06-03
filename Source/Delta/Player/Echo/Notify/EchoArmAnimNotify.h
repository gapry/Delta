// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "EchoBaseAnimNotify.h"
#include "EchoArmAnimNotify.generated.h"

UCLASS()
class DELTA_API UEchoArmAnimNotify : public UEchoBaseAnimNotify {
  GENERATED_BODY()

protected:
  virtual void ExecuteNotifyAction() override;
};
