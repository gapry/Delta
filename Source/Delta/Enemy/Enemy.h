// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyState.h"
#include "../Character/BaseCharacter.h"
#include "../Interface/HitInterface.h"
#include "Enemy.generated.h"

#define DELTA_ENEMY_ENABLE_DEBUG_HIT 0

class UAnimMontage;
class UAttributeComponent;
class UHealthBarComponent;

UCLASS()
class DELTA_API AEnemy : public ABaseCharacter, public IHitInterface {
  GENERATED_BODY()

public:
  AEnemy();

  virtual void Tick(float DeltaTime) override;

  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  virtual void GetHit(const FVector& ImpactPoint) override;

  virtual float TakeDamage(float                      DamageAmount,    //
                           struct FDamageEvent const& DamageEvent,     //
                           class AController*         EventInstigator, //
                           AActor*                    DamageCauser) override;

  void PlayHitReactMontage(const FName& SectionName);

  void DirectionalHitReact(const FVector& ImpactPoint);

  void HideHealthBar();
  void ShowHealthBar();

protected:
  virtual void BeginPlay() override;

  void Die();

  UPROPERTY(EditDefaultsOnly, Category = "Montages")
  TObjectPtr<UAnimMontage> HitReactMontage;

  UPROPERTY(EditDefaultsOnly, Category = "Montages")
  TObjectPtr<UAnimMontage> DeathMontage;

  UPROPERTY(VisibleAnywhere, Category = "Attributes")
  TObjectPtr<UAttributeComponent> AttributeComponent;

  UPROPERTY(EditAnywhere, Category = "Widgets")
  TObjectPtr<UHealthBarComponent> HealthBarComponent;

  UPROPERTY(BlueprintReadOnly)
  EDeathPose DeathPose = EDeathPose::EDP_Alive;

  UPROPERTY()
  TObjectPtr<AActor> CombatTarget;

  UPROPERTY()
  TObjectPtr<AActor> MoveTarget;

  UPROPERTY(EditAnywhere)
  double CombatRadius{500.f};

  UPROPERTY();
  float DeathLifeSpanSeconds{10.f};

private:
  void VerifyAIMoveToLocation(const FVector& Location);
  void VerifyAIMoveToTargetPointByTag(const FName& TargetTag);

  void VerifyAISetTheCombatTarget();
  void VerifyAIMoveToCombatTarget();
};
