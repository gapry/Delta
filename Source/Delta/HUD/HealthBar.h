// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

class UProgressBar;
class UCanvasPanelSlot;

UCLASS()
class DELTA_API UHealthBar : public UUserWidget {
  GENERATED_BODY()

public:
  UHealthBar(const FObjectInitializer& ObjectInitializer);

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UProgressBar> HealthBar;

  UPROPERTY()
  TObjectPtr<UCanvasPanelSlot> CanvasSlot;

protected:
  virtual void NativeConstruct() override;
  virtual void NativePreConstruct() override;
};
