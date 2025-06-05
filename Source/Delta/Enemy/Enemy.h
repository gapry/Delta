// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Character/BaseCharacter.h"
#include "../Interface/HitInterface.h"
#include "Enemy.generated.h"

#define DELTA_ENEMY_ENABLE_DEBUG_HIT 0

class UAnimMontage;
class UAttributeComponent;

UCLASS()
class DELTA_API AEnemy : public ABaseCharacter, public IHitInterface {
  GENERATED_BODY()

public:
  AEnemy();

  virtual void Tick(float DeltaTime) override;

  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  virtual void GetHit(const FVector& ImpactPoint) override;

  void PlayHitReactMontage(const FName& SectionName);

  void DirectionalHitReact(const FVector& ImpactPoint);

protected:
  virtual void BeginPlay() override;

  UPROPERTY(EditDefaultsOnly, Category = "Montages")
  TObjectPtr<UAnimMontage> HitReactMontage;

  UPROPERTY(VisibleAnywhere, Category = "Attributes")
  TObjectPtr<UAttributeComponent> Attributes;
};
