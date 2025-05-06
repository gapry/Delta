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
  void SetForwardDirection(const FVector& NewForwardDirection);

  void RenderDebugShape() const;

protected:
  virtual void BeginPlay() override;

private:
  UPROPERTY(EditAnywhere, Category = "Item")
  FVector CurrentLocation;

  UPROPERTY(EditAnywhere, Category = "Item")
  FVector ForwardDirection;

  UPROPERTY(EditAnywhere, Category = "Item")
  FRotator CurrentRotation;
};
