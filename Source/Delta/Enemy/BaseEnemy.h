// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyState.h"
#include "Engine/TimerHandle.h"
#include "Perception/AIPerceptionTypes.h"
#include "../Character/BaseCharacter.h"
#include "BaseEnemy.generated.h"

#define DELTA_ENEMY_ENABLE_DEBUG_HIT              0
#define DELTA_ENEMY_ENABLE_DEBUG_IN_TARGET_RANGE  0
#define DELTA_ENEMY_ENABLE_DEBUG_BEGIN_NAVIGATION 0

class UAnimMontage;
class UAttributeComponent;
class UHealthBarComponent;
class AAIController;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class AWeapon;

UCLASS()
class DELTA_API ABaseEnemy : public ABaseCharacter {
  GENERATED_BODY()

public:
  ABaseEnemy();

  virtual void Tick(float DeltaTime) override;

  virtual void GetHit(const FVector& ImpactPoint) override;

  virtual float TakeDamage(float                      DamageAmount,    //
                           struct FDamageEvent const& DamageEvent,     //
                           class AController*         EventInstigator, //
                           AActor*                    DamageCauser) override;

  virtual void Destroyed() override;

  void SetPatrolTargets(const FName& TargetTag);

  void HideHealthBar();

  void ShowHealthBar();

  void PatrolTimerFinished();

  void CheckPatrolTarget();

  void CheckCombatTarget();

  void LoseInterest();

  void StartPatrolling();

  bool IsOutsideCombatRadius();

  void ChaseTarget();

  bool IsOutsideAttackRadius();

  bool IsChasing() const;

  bool IsInsideAttackRadius();

  bool IsAttacking() const;

  void StartAttackTimer();

  void ClearAttackTimer();

  void ClearPatrolTimer();

  bool IsDead() const;

  bool IsEngaged() const;

  FTimerHandle AttackTimer;

  UPROPERTY(EditAnywhere, Category = "Combat")
  float AttackMin = 0.5f;

  UPROPERTY(EditAnywhere, Category = "Combat")
  float AttackMax = 1.f;

  EDeathPose  DeathPose  = EDeathPose::EDP_Alive;
  EEnemyState EnemyState = EEnemyState::EES_Patrolling;

protected:
  virtual void BeginPlay() override;

  virtual void PostInitializeComponents() override;

  void Attack();

  virtual void Die() override;

  virtual void PlayAttackMontage() override;

  bool InTargetRange(AActor* Target, double Radius);

  void MoveToTarget(AActor* Target, const float AcceptedRadius = 50.0f);

  AActor* ChoosePatrolTarget();

  UFUNCTION()
  void PawnSeen(AActor* Actor, FAIStimulus Stimulus);

  UPROPERTY(EditAnywhere, Category = "Widgets")
  TObjectPtr<UHealthBarComponent> HealthBarComponent;

  UPROPERTY()
  TObjectPtr<AActor> CombatTarget;

  UPROPERTY()
  TObjectPtr<AAIController> EnemyController;

  UPROPERTY(EditInstanceOnly, Category = "AI Navigation", BlueprintReadWrite)
  TObjectPtr<AActor> PatrolTarget;

  UPROPERTY(EditInstanceOnly, Category = "AI Navigation", BlueprintReadWrite)
  TArray<TObjectPtr<AActor>> PatrolTargets;

  UPROPERTY()
  TObjectPtr<AActor> MoveTargetPoint;

  UPROPERTY()
  TObjectPtr<AActor> MoveTargetPlayer;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Navigation")
  TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

  UPROPERTY(EditAnywhere, Category = "AI Navigation")
  TObjectPtr<UAISenseConfig_Sight> SightConfig;

  UPROPERTY(EditAnywhere, Category = "AI Navigation")
  double CombatRadius{500.f};

  UPROPERTY(EditAnywhere, Category = "AI Navigation")
  double PatrolRadius{100.f};

  UPROPERTY(EditAnywhere, Category = "AI Navigation")
  double AttackRadius{150.f};

  UPROPERTY(EditAnywhere, Category = "AI Navigation");
  float DeathLifeSpanSeconds{10.f};

  UPROPERTY(EditAnywhere, Category = "AI Navigation")
  float WaitMin{5.f};

  UPROPERTY(EditAnywhere, Category = "AI Navigation")
  float WaitMax{10.f};

  UPROPERTY(EditAnywhere)
  float NormalSpeed{75.f};

  UPROPERTY(EditAnywhere)
  float UpperBoundSpeed{300.f};

  UPROPERTY(EditAnywhere, Category = "Combat")
  float PatrollingSpeed{125.f};

  UPROPERTY(EditAnywhere, Category = "Combat")
  float ChasingSpeed{300.f};

  UPROPERTY();
  FTimerHandle PatrolTimer;

  UPROPERTY(EditAnywhere)
  TSubclassOf<class AWeapon> WeaponClass;

private:
  void VerifyAIMoveToLocation(const FVector& Location);
  void VerifyAIMoveToTargetPointByTag(const FName& TargetTag);

  void VerifyAISetToMoveTargetPlayer();
  void VerifyAIMoveToMoveTargetPlayer();
  void VerifyAIMoveNavigationPath();

  FVector LastTargetLocation;
};
