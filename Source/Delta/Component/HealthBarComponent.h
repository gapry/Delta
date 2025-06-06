// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

class UHealthBar;

UCLASS()
class DELTA_API UHealthBarComponent : public UWidgetComponent {
  GENERATED_BODY()

public:
  void SetHealthPercent(const float Percent);

private:
  UPROPERTY();
  TObjectPtr<UHealthBar> HealthBarWidget;
};
