// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "ControlRig.h"
#include "EchoControlRig.generated.h"

UCLASS(Blueprintable)
class DELTA_API UEchoControlRig : public UControlRig {
  GENERATED_BODY()

protected:
  virtual void Initialize(bool bRequestInit) override;
  virtual bool Execute(const FName& InEventName) override;
};
