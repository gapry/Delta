// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DELTA_API UAttributeComponent : public UActorComponent {
  GENERATED_BODY()

public:
  UAttributeComponent();

  virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

  void ReceiveDamage(const float DamageAmount);

  void SetHealth(const float NewHealth);

  float GetHealth() const;

  float GetHealthPercent() const;

protected:
  virtual void BeginPlay() override;

  UPROPERTY(EditAnywhere, Category = "Actor Attributes")
  float MaximumHealth{100.0f};

  UPROPERTY(EditAnywhere, Category = "Actor Attributes")
  float CurrentHealth{MaximumHealth};
};
