// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface {
  GENERATED_BODY()
};

class DELTA_API IHitInterface {
  GENERATED_BODY()

public:
  virtual void GetHit(const FVector& ImpactPoint) = 0;
};
