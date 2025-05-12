// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "QuadPyramid.generated.h"

UCLASS()
class DELTA_API AQuadPyramid : public AItem {
  GENERATED_BODY()

public:
  AQuadPyramid();

  virtual void TickAction(const float DeltaTime) override;

protected:
  virtual void BeginPlayAction() override;
};
