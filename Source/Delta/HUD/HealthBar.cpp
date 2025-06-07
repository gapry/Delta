// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "HealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanelSlot.h"

UHealthBar::UHealthBar(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer) {
}

void UHealthBar::NativeConstruct() {
  Super::NativeConstruct();
}

void UHealthBar::NativePreConstruct() {
  Super::NativePreConstruct();

  if (HealthBar) {
    HealthBar->SetBarFillType(EProgressBarFillType::Type::LeftToRight);
    HealthBar->SetPercent(1.0f);

    CanvasSlot = Cast<UCanvasPanelSlot>(HealthBar->Slot);
    if (CanvasSlot) {
      CanvasSlot->SetSize(FVector2D(180.0f, 13.0f));
      CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
      CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
    }
  }
}
