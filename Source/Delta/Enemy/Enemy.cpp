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
#include "../Common/Finder.h"
#include "../Common/LogUtil.h"
#include "../Common/DebugShape.h"
#include "../Component/AttributeComponent.h"
#include "../Component/HealthBarComponent.h"
#include "../Player/Echo/EchoCharacter.h"

AEnemy::AEnemy() {
  {
    PrimaryActorTick.bCanEverTick = true;
  }

  {
    if (UCharacterMovementComponent* const MoveComponent = GetCharacterMovement()) {
      MoveComponent->bUseRVOAvoidance          = true;
      MoveComponent->bOrientRotationToMovement = true;
      MoveComponent->MaxWalkSpeed              = 125.0f;

      bUseControllerRotationPitch = false;
      bUseControllerRotationYaw   = false;
      bUseControllerRotationRoll  = false;
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
    AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
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
}

void AEnemy::BeginPlay() {
  Super::BeginPlay();

  HideHealthBar();
  VerifyAISetToMoveTargetPlayer();
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

void AEnemy::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  VerifyAIMoveToMoveTargetPlayer();

  if (CombatTarget) {
    const double DistanceToTarget = (CombatTarget->GetActorLocation() - GetActorLocation()).Size();
    if (DistanceToTarget > CombatRadius) {
      CombatTarget = nullptr;
      HideHealthBar();
    }
  }
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);
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

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
  if (AttributeComponent) {
    AttributeComponent->ReceiveDamage(DamageAmount);
    if (HealthBarComponent) {
      HealthBarComponent->SetHealthPercent(AttributeComponent->GetHealthPercent());
    }
  }
  CombatTarget = EventInstigator->GetPawn();
  return DamageAmount;
}

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint) {
  const FVector Forward = GetActorForwardVector();

  const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
  const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

  const double CosTheta = FVector::DotProduct(Forward, ToHit);
  double       Theta    = FMath::RadiansToDegrees(FMath::Acos(CosTheta));

  const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
  if (CrossProduct.Z < 0) {
    Theta *= -1.f;
  }

#if DELTA_ENEMY_ENABLE_DEBUG_HIT
  UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);
  UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
  UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f, FColor::Blue, 5.f);
#endif

  FName Section("FromBack");
  if (Theta >= -45.f && Theta < 45.f) {
    Section = FName("FromFront");
  } else if (Theta >= -135.f && Theta < -45.f) {
    Section = FName("FromLeft");
  } else if (Theta >= 45.f && Theta < 135.f) {
    Section = FName("FromRight");
  }
  PlayHitReactMontage(Section);
}

void AEnemy::PlayHitReactMontage(const FName& SectionName) {
  UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
  if (AnimInstance && HitReactMontage) {
    AnimInstance->Montage_Play(HitReactMontage);
    AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
  }
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
