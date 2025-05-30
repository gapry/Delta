// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Character/BaseCharacter.h"
#include "../Interface/HitInterface.h"
#include "Enemy.generated.h"

UCLASS()
class DELTA_API AEnemy : public ABaseCharacter, public IHitInterface {
  GENERATED_BODY()

public:
  AEnemy();

  virtual void Tick(float DeltaTime) override;

  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  virtual void GetHit(const FVector& ImpactPoint) override;

protected:
  virtual void BeginPlay() override;
};
