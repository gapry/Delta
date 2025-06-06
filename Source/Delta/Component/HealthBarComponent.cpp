// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "HealthBarComponent.h"
#include "Components/ProgressBar.h"
#include "../HUD/HealthBar.h"

void UHealthBarComponent::SetHealthPercent(const float Percent) {
  if (HealthBarWidget == nullptr) {
    HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
  }

  if (HealthBarWidget && HealthBarWidget->HealthBar) {
    HealthBarWidget->HealthBar->SetPercent(Percent);
  }
}
