// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "Item.generated.h"

UCLASS()
class DELTA_API AItem : public AActor {
  GENERATED_BODY()

public:
  AItem();

  virtual void Tick(float DeltaTime) override;

  void SetLocation(const FVector& NewLocation);
  void SetRotation(const FRotator& NewRotation);
  void UpdateForwardDirection();

  void RenderDebugShape() const;
  void RenderDebugShapeOneFrame(const float DeltaTime);

protected:
  virtual void BeginPlay() override;

private:
  UPROPERTY(EditAnywhere, Category = "Item")
  float MovementRate{50.f};

  UPROPERTY(EditAnywhere, Category = "Item")
  float RotationRate{45.f};

  UPROPERTY(EditAnywhere, Category = "Item")
  FVector ForwardDirection{FVector::ForwardVector};
};
