// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "HealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Styling/SlateBrush.h"

UHealthBar::UHealthBar(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer) {
}

void UHealthBar::NativeConstruct() {
  Super::NativeConstruct();
}

void UHealthBar::NativePreConstruct() {
  Super::NativePreConstruct();

  if (HealthBar) {
    static const TCHAR* const Path          = TEXT("/Game/Texture/HealthBar/Medium_Health_Fill.Medium_Health_Fill");
    static auto* const        LoadedTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, Path));

    if (LoadedTexture != nullptr) {
      FillImage = LoadedTexture;
      FSlateBrush NewBrush;
      NewBrush.SetResourceObject(FillImage);
      HealthBar->WidgetStyle.SetFillImage(NewBrush);
      HealthBar->WidgetStyle.BackgroundImage.TintColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.0f);
    }

    HealthBar->SetBarFillType(EProgressBarFillType::Type::LeftToRight);
    HealthBar->SetPercent(1.0f);
    HealthBar->SetFillColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

    CanvasSlot = Cast<UCanvasPanelSlot>(HealthBar->Slot);
    if (CanvasSlot) {
      CanvasSlot->SetSize(FVector2D(180.0f, 13.0f));
      CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
      CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
    }
  }

  if (BorderImage) {
    static const TCHAR* const Path          = TEXT("/Game/Texture/HealthBar/Medium_Enemy_Front.Medium_Enemy_Front");
    static auto* const        LoadedTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, Path));

    if (LoadedTexture != nullptr) {
      BackgroundImage = LoadedTexture;
      FSlateBrush NewBrush;
      NewBrush.SetResourceObject(BackgroundImage);
      BorderImage->SetBrush(NewBrush);
    }

    CanvasSlot = Cast<UCanvasPanelSlot>(BorderImage->Slot);
    if (CanvasSlot) {
      CanvasSlot->SetSize(FVector2D(183.0f, 13.0f));
      CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
      CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
    }
  }
}
