// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "BaseEnemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/WidgetComponent.h"
#include "AIController.h"
#include "AITypes.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystemTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "../Common/Finder.h"
#include "../Common/LogUtil.h"
#include "../Common/DebugShape.h"
#include "../Component/AttributeComponent.h"
#include "../Component/HealthBarComponent.h"
#include "../Player/Echo/EchoCharacter.h"
#include "../Weapon/Weapon.h"

ABaseEnemy::ABaseEnemy() {
  {
    if (UCharacterMovementComponent* const MoveComponent = GetCharacterMovement()) {
      MoveComponent->bUseRVOAvoidance          = true;
      MoveComponent->bOrientRotationToMovement = true;
      MoveComponent->MaxWalkSpeed              = NormalSpeed;
    }
  }

  {
    AutoPossessPlayer = EAutoReceiveInput::Disabled;
    AutoPossessAI     = EAutoPossessAI::PlacedInWorldOrSpawned;
  }

  {
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
    SightConfig           = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

    SightConfig->SightRadius                  = 4000.f;
    SightConfig->LoseSightRadius              = 4200.f;
    SightConfig->PeripheralVisionAngleDegrees = 45.f;
    SightConfig->SetMaxAge(5.f);
    SightConfig->DetectionByAffiliation.bDetectEnemies    = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals   = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    AIPerceptionComponent->ConfigureSense(*SightConfig);
    AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
  }
}

void ABaseEnemy::PatrolTimerFinished() {
  MoveToTarget(PatrolTarget);
}

void ABaseEnemy::BeginPlay() {
  Super::BeginPlay();

  HideHealthBar();
  SetPatrolTargets(FName(TEXT("TargetNode")));
  MoveToTarget(PatrolTarget);

  AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseEnemy::PawnSeen);
}

void ABaseEnemy::PostInitializeComponents() {
  Super::PostInitializeComponents();

  EnemyController = Cast<AAIController>(GetController());
}

bool ABaseEnemy::CanAttack() {
  return IsInsideAttackRadius() && !IsAttacking() && !IsDead();
}

void ABaseEnemy::Attack() {
  PlayAttackMontage();
}

void ABaseEnemy::SetPatrolTargets(const FName& TargetTag) {
  TArray<AActor*> FoundActors;
  UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATargetPoint::StaticClass(), TargetTag, FoundActors);
  PatrolTargets = FoundActors;

  const int32 NumPatrolTargets = PatrolTargets.Num();
  const int32 TargetSelection  = FMath::RandRange(0, NumPatrolTargets - 1);
  PatrolTarget                 = PatrolTargets[TargetSelection];
}

void ABaseEnemy::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  if (IsDead()) {
    return;
  }

  if (EnemyState != EEnemyState::EES_Patrolling) {
    CheckCombatTarget();
  } else {
    CheckPatrolTarget();
  }
}

void ABaseEnemy::CheckPatrolTarget() {
  if (InTargetRange(PatrolTarget, PatrolRadius)) {
    PatrolTarget = ChoosePatrolTarget();

    const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
    GetWorldTimerManager().SetTimer(PatrolTimer, this, &ABaseEnemy::PatrolTimerFinished, WaitTime);
  }
}

void ABaseEnemy::CheckCombatTarget() {
  if (IsOutsideCombatRadius()) {
    ClearAttackTimer();
    LoseInterest();
    if (!IsEngaged()) {
      StartPatrolling();
    }
  } else if (IsOutsideAttackRadius() && !IsChasing()) {
    ClearAttackTimer();
    if (!IsEngaged()) {
      ChaseTarget();
    }
  } else if (CanAttack()) {
    StartAttackTimer();
  }
}

void ABaseEnemy::LoseInterest() {
  CombatTarget = nullptr;
  HideHealthBar();
}

void ABaseEnemy::StartPatrolling() {
  EnemyState                           = EEnemyState::EES_Patrolling;
  GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
  MoveToTarget(PatrolTarget);
}

bool ABaseEnemy::IsOutsideCombatRadius() {
  return !InTargetRange(CombatTarget, CombatRadius);
}

void ABaseEnemy::ChaseTarget() {
  EnemyState                           = EEnemyState::EES_Chasing;
  GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
  MoveToTarget(CombatTarget);
}

bool ABaseEnemy::IsOutsideAttackRadius() {
  return !InTargetRange(CombatTarget, AttackRadius);
}

bool ABaseEnemy::IsChasing() const {
  return EnemyState == EEnemyState::EES_Chasing;
}

bool ABaseEnemy::IsInsideAttackRadius() {
  return InTargetRange(CombatTarget, AttackRadius);
}

bool ABaseEnemy::IsAttacking() const {
  return EnemyState == EEnemyState::EES_Attacking;
}

void ABaseEnemy::StartAttackTimer() {
  EnemyState             = EEnemyState::EES_Attacking;
  const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
  GetWorldTimerManager().SetTimer(AttackTimer, this, &ABaseEnemy::Attack, AttackTime);
}

void ABaseEnemy::ClearAttackTimer() {
  GetWorldTimerManager().ClearTimer(AttackTimer);
}

void ABaseEnemy::ClearPatrolTimer() {
  GetWorldTimerManager().ClearTimer(PatrolTimer);
}

bool ABaseEnemy::IsDead() const {
  return EnemyState == EEnemyState::EES_Dead;
}

bool ABaseEnemy::IsEngaged() const {
  return EnemyState == EEnemyState::EES_Engaged;
}

bool ABaseEnemy::InTargetRange(AActor* Target, double Radius) {
  if (Target == nullptr) {
    return false;
  }

  const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();

#if DELTA_ENEMY_ENABLE_DEBUG_IN_TARGET_RANGE
  DELTA_DEBUG_SPHERE_ONE_FRAME(GetActorLocation());
  DELTA_DEBUG_SPHERE_ONE_FRAME(Target->GetActorLocation());
#endif

  return DistanceToTarget <= Radius;
}

void ABaseEnemy::MoveToTarget(AActor* Target, const float AcceptedRadius) {
  if (EnemyController == nullptr || Target == nullptr) {
    DELTA_LOG("EnemyController or Target is null for {}", TCHAR_TO_UTF8(*GetName()));
    return;
  }

  FAIMoveRequest MoveRequest;
  MoveRequest.SetGoalActor(Target);
  MoveRequest.SetAcceptanceRadius(AcceptedRadius);

#if DELTA_ENEMY_ENABLE_DEBUG_BEGIN_NAVIGATION
  FNavPathSharedPtr NavPath;
  if (NavPath.IsValid()) {
    EnemyController->MoveTo(MoveRequest, &NavPath);

    TArray<FNavPathPoint>& PathPoints = NavPath->GetPathPoints();
    for (auto& Point : PathPoints) {
      const FVector& Location = Point.Location;
      DrawDebugSphere(GetWorld(), Location, 12.f, 12, FColor::Green, false, 10.f);
    }
  }
#else
  EnemyController->MoveTo(MoveRequest);
#endif
}

AActor* ABaseEnemy::ChoosePatrolTarget() {
  TArray<AActor*> ValidTargets;
  for (AActor* Target : PatrolTargets) {
    if (Target != PatrolTarget) {
      ValidTargets.AddUnique(Target);
    }
  }

  if (const int32 NumPatrolTargets = ValidTargets.Num(); NumPatrolTargets > 0) {
    const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
    return ValidTargets[TargetSelection];
  }
  return nullptr;
}

void ABaseEnemy::HandleDamage(const float DamageAmount) {
  Super::HandleDamage(DamageAmount);

  if (AttributeComponent && HealthBarComponent) {
    HealthBarComponent->SetHealthPercent(AttributeComponent->GetHealthPercent());
  }
}

void ABaseEnemy::PawnSeen(AActor* ActorSeen, FAIStimulus Stimulus) {
  const bool bShouldChaseTarget = EnemyState != EEnemyState::EES_Dead &&            //
                                  EnemyState != EEnemyState::EES_Chasing &&         //
                                  EnemyState < EEnemyState::EES_Attacking &&        //
                                  ActorSeen->ActorHasTag(FName("EchoCharacter")) && //
                                  Stimulus.WasSuccessfullySensed();                 //

  if (bShouldChaseTarget) {
    CombatTarget = ActorSeen;
    ClearPatrolTimer();
    ChaseTarget();
  }
}

void ABaseEnemy::Die() {
  UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

  if (AnimInstance && DeathMontage) {
    AnimInstance->Montage_Play(DeathMontage);

    const int32 Selection   = FMath::RandRange(0, 5);
    FName       SectionName = FName();
    switch (Selection) {
      case 0: {
        SectionName = FName("FallingBack");
        DeathPose   = EDeathPose::EDP_Death_FallingBack;
      } break;
      case 1: {
        SectionName = FName("FallingForward");
        DeathPose   = EDeathPose::EDP_Death_FallingForward;
      } break;
      case 2: {
        SectionName = FName("FlyingBack");
        DeathPose   = EDeathPose::EDP_Death_FlyingBack;
      } break;
      case 3: {
        SectionName = FName("ToRight");
        DeathPose   = EDeathPose::EDP_Death_ToRight;
      } break;
      case 4: {
        SectionName = FName("ToFront");
        DeathPose   = EDeathPose::EDP_Death_ToFront;
      } break;
      case 5: {
        SectionName = FName("ToBack");
        DeathPose   = EDeathPose::EDP_Death_ToBack;
      } break;
      default: break;
    }
    AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);

    HideHealthBar();
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SetLifeSpan(DeathLifeSpanSeconds);
  }
}

void ABaseEnemy::PlayAttackMontage() {
}

float ABaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
  CombatTarget = EventInstigator->GetPawn();
  ChaseTarget();
  HandleDamage(DamageAmount);
  return DamageAmount;
}

void ABaseEnemy::Destroyed() {
  Super::Destroyed();

  if (EquippedWeapon) {
    EquippedWeapon->Destroy();
  }
}

void ABaseEnemy::GetHit(const FVector& ImpactPoint) {
#if DELTA_ENEMY_ENABLE_DEBUG_HIT
  DELTA_DEBUG_SPHERE_COLOR(ImpactPoint, FColor::Orange);
#endif
  ShowHealthBar();

  if (IsAlive()) {
    DirectionalHitReact(ImpactPoint);
    return;
  }
  Die();
  PlayHitSound(ImpactPoint);
  SpawnHitParticles(ImpactPoint);
}

void ABaseEnemy::HideHealthBar() {
  if (HealthBarComponent != nullptr) {
    HealthBarComponent->SetVisibility(false);
  }
}

void ABaseEnemy::ShowHealthBar() {
  if (HealthBarComponent != nullptr) {
    HealthBarComponent->SetVisibility(true);
  }
}
