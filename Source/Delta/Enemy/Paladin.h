// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "Paladin.generated.h"

UCLASS()
class DELTA_API APaladin : public ABaseEnemy {
  GENERATED_BODY()

public:
  APaladin();

  UPROPERTY(EditAnywhere, Category = "Item")
  TObjectPtr<UStaticMeshComponent> Shield;

protected:
  virtual void PostInitializeComponents() override;
};
