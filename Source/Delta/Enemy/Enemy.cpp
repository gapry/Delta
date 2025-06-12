// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Enemy.h"
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

AEnemy::AEnemy() {
  {
    if (UCharacterMovementComponent* const MoveComponent = GetCharacterMovement()) {
      MoveComponent->bUseRVOAvoidance          = true;
      MoveComponent->bOrientRotationToMovement = true;
      MoveComponent->MaxWalkSpeed              = NormalSpeed;
    }
  }

  {
    SkeletalMeshComponent = GetMesh();

    static constexpr const TCHAR* const SkeletalMeshPath{TEXT("/Script/Engine.SkeletalMesh'/Game/Mixamo/Paladin/"
                                                              "Sword_And_Shield_Idle.Sword_And_Shield_Idle'")};

    DELTA_SET_SKELETAL_MESH(SkeletalMeshComponent.Get(), SkeletalMeshPath);

    SkeletalMeshComponent->SetRelativeTransform(FTransform(FRotator(0.f, -90.f, 0.f), FVector(0.f, 0.f, -88.0f), FVector(1.f, 1.f, 1.f)));

    SkeletalMeshComponent->SetGenerateOverlapEvents(true);

    SkeletalMeshComponent->SetCollisionProfileName(TEXT("Custom"));
    SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SkeletalMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
    SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
  }

  {
    CapsuleComponent = GetCapsuleComponent();

    CapsuleComponent->SetCollisionProfileName(TEXT("Custom"));
    CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
    CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
  }

  {
    static constexpr const TCHAR* const AnimBlueprintPath{
      TEXT("/Script/Engine.AnimBlueprint'/Game/Delta/Enemy/Animation/Blueprint/ABP_Enemy.ABP_Enemy_C'")};
    DELTA_SET_ANIMATION_BLUEPRINT(SkeletalMeshComponent.Get(), AnimBlueprintPath);
  }

  {
    static constexpr const TCHAR* const MontagePath{TEXT("/Script/Engine.AnimMontage'/Game/Delta/Enemy/Animation/Montage/AM_HitReact.AM_HitReact'")};
    DELTA_SET_ANIMATION_MONTAGE(HitReactMontage, MontagePath);
  }

  {
    static constexpr const TCHAR* const MontagePath{TEXT("/Script/Engine.AnimMontage'/Game/Delta/Enemy/Animation/Montage/AM_Death.AM_Death'")};
    DELTA_SET_ANIMATION_MONTAGE(DeathMontage, MontagePath);
  }

  {
    HealthBarComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
    HealthBarComponent->SetupAttachment(GetRootComponent());
    HealthBarComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));
    HealthBarComponent->SetWidgetSpace(EWidgetSpace::Screen);

    static constexpr const TCHAR* const Path{TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Delta/HUD/WBP_HealthBar.WBP_HealthBar_C'")};
    DELTA_SET_USER_WIDGET(HealthBarComponent, Path);
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

void AEnemy::PatrolTimerFinished() {
  MoveToTarget(PatrolTarget);
}

void AEnemy::BeginPlay() {
  Super::BeginPlay();

  HideHealthBar();
  SetPatrolTargets(FName(TEXT("TargetNode")));
  MoveToTarget(PatrolTarget);

  AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemy::PawnSeen);
}

void AEnemy::PostInitializeComponents() {
  Super::PostInitializeComponents();

  EnemyController = Cast<AAIController>(GetController());
}

void AEnemy::Attack() {
}

void AEnemy::SetPatrolTargets(const FName& TargetTag) {
  TArray<AActor*> FoundActors;
  UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATargetPoint::StaticClass(), TargetTag, FoundActors);
  PatrolTargets = FoundActors;

  const int32 NumPatrolTargets = PatrolTargets.Num();
  const int32 TargetSelection  = FMath::RandRange(0, NumPatrolTargets - 1);
  PatrolTarget                 = PatrolTargets[TargetSelection];
}

void AEnemy::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  if (EnemyState != EEnemyState::EES_Patrolling) {
    CheckCombatTarget();
  } else {
    CheckPatrolTarget();
  }
}

void AEnemy::CheckPatrolTarget() {
  if (InTargetRange(PatrolTarget, PatrolRadius)) {
    PatrolTarget = ChoosePatrolTarget();

    const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
    GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);
  }
}

void AEnemy::CheckCombatTarget() {
  if (!InTargetRange(CombatTarget, CombatRadius)) {
    CombatTarget                         = nullptr;
    EnemyState                           = EEnemyState::EES_Patrolling;
    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

    HideHealthBar();
    MoveToTarget(PatrolTarget);
  } else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing) {
    EnemyState                           = EEnemyState::EES_Chasing;
    GetCharacterMovement()->MaxWalkSpeed = UpperBoundSpeed;
    MoveToTarget(CombatTarget);
  } else if (InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Attacking) {
    EnemyState = EEnemyState::EES_Attacking;
    DELTA_LOG("Enemy {} is attacking target {}", TCHAR_TO_UTF8(*GetName()), TCHAR_TO_UTF8(*CombatTarget->GetName()));
  }
}

bool AEnemy::InTargetRange(AActor* Target, double Radius) {
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

void AEnemy::MoveToTarget(AActor* Target, const float AcceptedRadius) {
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

AActor* AEnemy::ChoosePatrolTarget() {
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

void AEnemy::PawnSeen(AActor* ActorSeen, FAIStimulus Stimulus) {
  if (EnemyState == EEnemyState::EES_Chasing) {
    return;
  }

  if (Stimulus.WasSuccessfullySensed() && ActorSeen->ActorHasTag(FName("EchoCharacter"))) {
    GetWorldTimerManager().ClearTimer(PatrolTimer);
    GetCharacterMovement()->MaxWalkSpeed = UpperBoundSpeed;
    CombatTarget                         = ActorSeen;

    if (EnemyState != EEnemyState::EES_Attacking) {
      EnemyState = EEnemyState::EES_Chasing;
      MoveToTarget(CombatTarget);
    }
  }
}

void AEnemy::Die() {
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

void AEnemy::PlayAttackMontage() {
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
  if (AttributeComponent) {
    AttributeComponent->ReceiveDamage(DamageAmount);
    if (HealthBarComponent) {
      HealthBarComponent->SetHealthPercent(AttributeComponent->GetHealthPercent());
    }
  }
  CombatTarget                         = EventInstigator->GetPawn();
  EnemyState                           = EEnemyState::EES_Chasing;
  GetCharacterMovement()->MaxWalkSpeed = UpperBoundSpeed;
  MoveToTarget(CombatTarget);
  return DamageAmount;
}

void AEnemy::GetHit(const FVector& ImpactPoint) {
#if DELTA_ENEMY_ENABLE_DEBUG_HIT
  DELTA_DEBUG_SPHERE_COLOR(ImpactPoint, FColor::Orange);
#endif
  ShowHealthBar();

  if (AttributeComponent != nullptr && AttributeComponent->GetHealth() > 0.0f) {
    DirectionalHitReact(ImpactPoint);
    return;
  }
  Die();
}

void AEnemy::HideHealthBar() {
  if (HealthBarComponent != nullptr) {
    HealthBarComponent->SetVisibility(false);
  }
}

void AEnemy::ShowHealthBar() {
  if (HealthBarComponent != nullptr) {
    HealthBarComponent->SetVisibility(true);
  }
}

void AEnemy::VerifyAIMoveToLocation(const FVector& TargetLocation) {
  auto* const AIController = Cast<AAIController>(GetController());
  if (AIController) {
    FAIMoveRequest MoveRequest;
    MoveRequest.SetGoalLocation(TargetLocation);
    MoveRequest.SetAcceptanceRadius(5.0f);
    MoveRequest.SetUsePathfinding(true);
    MoveRequest.SetAllowPartialPath(true);

    if (AIController->GetPathFollowingComponent()) {
      ACharacter*                  Character         = AIController->GetCharacter();
      UCharacterMovementComponent* MovementComponent = nullptr;
      if (Character) {
        MovementComponent = Character->GetCharacterMovement();
      }
      // warning C4996: 'UPathFollowingComponent::SetMovementComponent':
      // SetMovementComponent(UNavMovementComponent* MoveComp) is deprecated, please use SetNavMoveInterface(INavMoveInterface* NavMoveInterface)
      // instead. Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile.
      AIController->GetPathFollowingComponent()->SetMovementComponent(MovementComponent);

      if (Character && MovementComponent) {
        if (AIController->GetCharacter() && AIController->GetCharacter()->GetCharacterMovement()) {
          AIController->GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = 100.f;
        }
      }
      FNavPathSharedPtr                 NavPath;
      EPathFollowingRequestResult::Type MoveResult = AIController->MoveTo(MoveRequest, &NavPath);
      DELTA_LOG("MoveResult = {}", DeltaFormat("{}", static_cast<int32>(MoveResult)));
    }
  }
}

void AEnemy::VerifyAIMoveToTargetPointByTag(const FName& TargetTag) {
  for (TActorIterator<AActor> It(GetWorld()); It; ++It) {
    if (It->ActorHasTag(TargetTag)) {
      MoveTargetPoint = *It;
      break;
    }
  }

  if (MoveTargetPoint) {
    auto* AIController = Cast<AAIController>(GetController());
    if (AIController) {
      FAIMoveRequest MoveRequest;
      MoveRequest.SetGoalActor(MoveTargetPoint);
      MoveRequest.SetAcceptanceRadius(5.0f);
      MoveRequest.SetUsePathfinding(true);
      MoveRequest.SetAllowPartialPath(true);

      FNavPathSharedPtr NavPath;
      AIController->MoveTo(MoveRequest, &NavPath);
    }
  }
}

void AEnemy::VerifyAISetToMoveTargetPlayer() {
  auto* const PlayerCharacter = Cast<AEchoCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
  if (PlayerCharacter) {
    MoveTargetPlayer = PlayerCharacter;
    UE_LOG(LogTemp, Warning, TEXT("%s assigned CombatTarget: %s"), *GetName(), *MoveTargetPlayer->GetName());
  }
}

void AEnemy::VerifyAIMoveToMoveTargetPlayer() {
  if (!MoveTargetPlayer) {
    return;
  }

  auto* AIController = Cast<AAIController>(GetController());
  if (!AIController) {
    return;
  }

  const float AcceptanceRadius = 5.0f;

  FAIMoveRequest MoveRequest;
  MoveRequest.SetGoalActor(MoveTargetPlayer);
  MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
  MoveRequest.SetUsePathfinding(true);
  MoveRequest.SetAllowPartialPath(true);

  FNavPathSharedPtr                 NavPath;
  EPathFollowingRequestResult::Type MoveResult = AIController->MoveTo(MoveRequest, &NavPath);
}

void AEnemy::VerifyAIMoveNavigationPath() {
  EnemyController = Cast<AAIController>(GetController());

  if (EnemyController == nullptr) {
    DELTA_LOG("{}", DeltaFormat("EnemyController is null for {}", TCHAR_TO_UTF8(*GetName())));
    return;
  }

  const FName     TargetTag = FName(TEXT("TargetNode"));
  TArray<AActor*> FoundActors;
  UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATargetPoint::StaticClass(), TargetTag, FoundActors);
  if (FoundActors.Num() > 0) {
    PatrolTarget = Cast<ATargetPoint>(FoundActors[0]);
  }

  if (EnemyController != nullptr && PatrolTarget != nullptr) {
    FAIMoveRequest MoveRequest;
    MoveRequest.SetGoalActor(PatrolTarget);
    MoveRequest.SetAcceptanceRadius(100.f);

    FNavPathSharedPtr NavPath;
    EnemyController->MoveTo(MoveRequest, &NavPath);

    if (NavPath.IsValid()) {
      TArray<FNavPathPoint>& PathPoints = NavPath->GetPathPoints();
      for (auto& Point : PathPoints) {
        const FVector& Location = Point.Location;
        DrawDebugSphere(GetWorld(), Location, 20.f, 32, FColor::Red, true);
      }
    }
  }
}
