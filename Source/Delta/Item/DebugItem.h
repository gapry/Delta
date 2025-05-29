// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "DebugItem.generated.h"

UCLASS()
class DELTA_API ADebugItem : public AItem {
  GENERATED_BODY()

public:
  void RenderDebugShape() const;
  void RenderDebugShapeOneFrame(const float DeltaTime);

protected:
  virtual void BeginPlayAction() override;
  virtual void TickAction(const float DeltaTime) override;
};
