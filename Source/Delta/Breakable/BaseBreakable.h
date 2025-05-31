// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "BaseBreakable.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class DELTA_API ABaseBreakable : public AActor {
  GENERATED_BODY()

public:
  ABaseBreakable();

  virtual void Tick(float DeltaTime) override;

protected:
  virtual void BeginPlay() override;

  UPROPERTY(EditAnywhere)
  TObjectPtr<UGeometryCollectionComponent> GeometryCollectionComponent{nullptr};
};
