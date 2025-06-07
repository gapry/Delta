// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

class UProgressBar;
class UCanvasPanelSlot;
class UImage;

UCLASS()
class DELTA_API UHealthBar : public UUserWidget {
  GENERATED_BODY()

public:
  UHealthBar(const FObjectInitializer& ObjectInitializer);

  UPROPERTY(meta = (BindWidget), EditAnywhere)
  TObjectPtr<UProgressBar> HealthBar;

  UPROPERTY(meta = (BindWidget), EditAnywhere)
  TObjectPtr<UImage> BorderImage;

  UPROPERTY()
  TObjectPtr<UCanvasPanelSlot> CanvasSlot;

protected:
  virtual void NativeConstruct() override;
  virtual void NativePreConstruct() override;

  UPROPERTY()
  TObjectPtr<UTexture2D> FillImage;

  UPROPERTY()
  TObjectPtr<UTexture2D> BackgroundImage;
};
