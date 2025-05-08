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

  void InitializeStaticMeshComponent(const FString& StaticMeshPath);

  virtual void InitializeCollision();
  virtual void InitializeRootComponent();

  virtual void Tick(float DeltaTime) override;
  virtual void TickAction(const float DeltaTime);

  void SetLocation(const FVector& NewLocation);
  void SetRotation(const FRotator& NewRotation);
  void UpdateForwardDirection();

  void RenderDebugShape() const;
  void RenderDebugShapeOneFrame(const float DeltaTime);

protected:
  virtual void BeginPlay() override;
  virtual void BeginPlayAction();

  UFUNCTION(BlueprintCallable, Category = "Item")
  float GetSineOscillationOffset() const;

  UFUNCTION(BlueprintCallable, Category = "Item")
  float GetCosineOscillationOffset() const;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
  TObjectPtr<UStaticMeshComponent> StaticMeshComponent{nullptr};

private:
  UPROPERTY(BlueprintReadWrite,
            EditAnywhere,
            Category = "Item",
            meta     = (AllowPrivateAccess = "true"))
  float MovementRate{50.f};

  UPROPERTY(BlueprintReadWrite,
            EditAnywhere,
            Category = "Item",
            meta     = (AllowPrivateAccess = "true"))
  float RotationRate{45.f};

  UPROPERTY(VisibleAnywhere, Category = "Item")
  FVector ForwardDirection{FVector::ForwardVector};

  UPROPERTY(BlueprintReadWrite,
            EditAnywhere,
            Category = "Item",
            meta     = (AllowPrivateAccess = "true"))
  float Amplitude{0.5f};

  UPROPERTY(BlueprintReadWrite,
            EditAnywhere,
            Category = "Item",
            meta     = (AllowPrivateAccess = "true"))
  float Frequency{0.5f};

  UPROPERTY(VisibleInstanceOnly,
            BlueprintReadOnly,
            Category = "Item",
            meta     = (AllowPrivateAccess = "true"))
  float RunningTime{0.f};
};
