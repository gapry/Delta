// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "HealthBarComponent.h"
#include "Components/ProgressBar.h"
#include "../HUD/HealthBar.h"
#include "../Common/LogUtil.h"

void UHealthBarComponent::SetHealthPercent(const float Percent) {
  if (Percent < 0.0f || Percent > 1.0f) {
    DELTA_LOG("{}", DeltaFormat("Health percent must be between 0.0 and 1.0, received: {}", Percent));
    return;
  }

  UUserWidget* CurrentWidget = GetUserWidgetObject();
  if (CurrentWidget == nullptr) {
    DELTA_LOG("{}", DeltaFormat("HealthBarComponent widget is not initialized or not set."));
    return;
  }

  if (HealthBarWidget == nullptr) {
    HealthBarWidget = Cast<UHealthBar>(CurrentWidget);
  }

  if (HealthBarWidget->HealthBar == nullptr) {
    DELTA_LOG("{}", DeltaFormat("HealthBarWidget's HealthBar is not initialized or not set."));
    return;
  }

  HealthBarWidget->HealthBar->SetPercent(Percent);
}
