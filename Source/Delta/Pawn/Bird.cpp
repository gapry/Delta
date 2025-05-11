// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Bird.h"
#include "../Common/Finder.h"
#include "../Common/LogUtil.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ABird::ABird() {
  PrimaryActorTick.bCanEverTick = true;

  bUseControllerRotationYaw   = true;
  bUseControllerRotationPitch = true;
  bUseControllerRotationRoll  = true;

  {
    static constexpr const TCHAR* const ComponentName = TEXT("SkeletalMeshComponent");
    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(ComponentName);

    static const TCHAR* const MeshPath =
      TEXT("SkeletalMesh'/Game/AnimalVarietyPack/Crow/Meshes/SK_Crow.SK_Crow'");
    Finder::SetSkeletalMesh(SkeletalMeshComponent, MeshPath);

    static constexpr const TCHAR* const AnimSequencePath =
      TEXT("AnimSequence'/Game/AnimalVarietyPack/Crow/Animations/ANIM_Crow_Fly.ANIM_Crow_Fly'");
    Finder::SetAnimation(SkeletalMeshComponent, AnimSequencePath);
  }

  {
    static constexpr const TCHAR* const CapsuleName = TEXT("CapsuleComponent");
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(CapsuleName);
  }

  {
    static constexpr const TCHAR* const IMC_Path =
      TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Delta/Pawn/Input/IMC_Bird.IMC_Bird'");
    InputMappingContext = Finder::FindInputMappingContext(IMC_Path);

    static constexpr const TCHAR* const IA_Move_Path =
      TEXT("/Script/EnhancedInput.InputAction'/Game/Delta/Pawn/Input/IA_Bird_Move.IA_Bird_Move'");
    MoveAction = Finder::FindInputAction(IA_Move_Path);

    static constexpr const TCHAR* const IA_Rotate_Path = TEXT(
      "/Script/EnhancedInput.InputAction'/Game/Delta/Pawn/Input/IA_Bird_Rotate.IA_Bird_Rotate'");
    RotateAction = Finder::FindInputAction(IA_Rotate_Path);

    static constexpr const TCHAR* const IA_Look_Path =
      TEXT("/Script/EnhancedInput.InputAction'/Game/Delta/Pawn/Input/IA_Bird_Look.IA_Bird_Look'");
    LookAction = Finder::FindInputAction(IA_Look_Path);
  }

  {
    static constexpr const TCHAR* const FloatingPawnMovementName = TEXT("FloatingPawnMovement");
    FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(FloatingPawnMovementName);
  }

  {
    static constexpr const TCHAR* const ComponentName = TEXT("SpringArm");
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(ComponentName);
  }

  {
    static constexpr const TCHAR* const ComponentName = TEXT("ViewCamera");

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(ComponentName);
    CameraComponent->SetupAttachment(SpringArmComponent);
  }

  {
    static constexpr const TCHAR* const RootComponentName = TEXT("RootComponent");
    RootComponent = CreateDefaultSubobject<USceneComponent>(RootComponentName);
  }

  {
    if (CapsuleComponent) {
      RootComponent = CapsuleComponent;

      if (SkeletalMeshComponent) {
        SkeletalMeshComponent->AttachToComponent(RootComponent,
                                                 FAttachmentTransformRules::KeepRelativeTransform);
      } else {
        DELTA_LOG("{}", DeltaFormat("SkeletalMeshComponent is null"));
      }

      if (FloatingPawnMovement) {
        FloatingPawnMovement->UpdatedComponent = RootComponent;
      } else {
        DELTA_LOG("{}", DeltaFormat("FloatingPawnMovement is null"));
      }

      if (SpringArmComponent) {
        SpringArmComponent->SetupAttachment(RootComponent);
      } else {
        DELTA_LOG("{}", DeltaFormat("SpringArmComponent is null"));
      }
    } else {
      DELTA_LOG("{}", DeltaFormat("CapsuleComponent is null"));
    }
  }
}

void ABird::BeginPlay() {
  Super::BeginPlay();

  const auto* const PlayerController = CastChecked<APlayerController>(Controller);

  auto* const Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
    PlayerController->GetLocalPlayer());
  if (!Subsystem) {
    DELTA_LOG("{}", DeltaFormat("Subsystem is null"));
    return;
  }

  if (!InputMappingContext) {
    DELTA_LOG("{}", DeltaFormat("InputMappingContext is null"));
    return;
  }

  static constexpr const int32 Priority = 0;
  Subsystem->AddMappingContext(InputMappingContext, Priority);
}

void ABird::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
}

void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  auto* const EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

  if (!MoveAction) {
    DELTA_LOG("{}", DeltaFormat("MoveAction is null"));
    return;
  }

  if (!RotateAction) {
    DELTA_LOG("{}", DeltaFormat("RotateAction is null"));
    return;
  }

  if (!LookAction) {
    DELTA_LOG("{}", DeltaFormat("LookAction is null"));
    return;
  }

  EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move);
  EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ABird::Rotate);
  EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABird::Look);
}

void ABird::PostInitializeComponents() {
  Super::PostInitializeComponents();

  PostInitializeSkeletalMeshComponent();
  PostInitializeCapsuleComponent();
  PostInitializeCollision();
  PostInitializeFloatingPawnMovement();
  PostInitializeSpringArmComponent();
  PostInitializeCameraComponent();
}

void ABird::PostInitializeSkeletalMeshComponent() {
  if (!SkeletalMeshComponent) {
    DELTA_LOG("{}", DeltaFormat("SkeletalMeshComponent is null"));
    return;
  }

  SkeletalMeshComponent->SetMobility(EComponentMobility::Movable);

  SkeletalMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -10.f));
  SkeletalMeshComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

  SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationSingleNode);
  SkeletalMeshComponent->AnimationData.bSavedPlaying = true;
  SkeletalMeshComponent->AnimationData.bSavedLooping = true;
}

void ABird::PostInitializeCapsuleComponent() {
  if (!CapsuleComponent) {
    DELTA_LOG("{}", DeltaFormat("CapsuleComponent is null"));
    return;
  }

  CapsuleComponent->SetCapsuleHalfHeight(20.f);
  CapsuleComponent->SetCapsuleRadius(15.f);
}

void ABird::PostInitializeCollision() {
  if (!CapsuleComponent) {
    DELTA_LOG("{}", DeltaFormat("CapsuleComponent is null"));
    return;
  }

  static constexpr const TCHAR* const CollisionProfileName = TEXT("BlockAll");

  CapsuleComponent->SetCollisionProfileName(CollisionProfileName);
  CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
  CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
  CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Block);
  CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

void ABird::PostInitializeFloatingPawnMovement() {
  if (!FloatingPawnMovement) {
    DELTA_LOG("{}", DeltaFormat("FloatingPawnMovement is null"));
    return;
  }

  FloatingPawnMovement->MaxSpeed     = 1024.f;
  FloatingPawnMovement->Acceleration = 2048.f;
}

void ABird::PostInitializeSpringArmComponent() {
  if (!SpringArmComponent) {
    DELTA_LOG("{}", DeltaFormat("SpringArmComponent is null"));
    return;
  }

  SpringArmComponent->TargetArmLength = 100.f;
}

void ABird::PostInitializeCameraComponent() {
  if (!CameraComponent) {
    DELTA_LOG("{}", DeltaFormat("CameraComponent is null"));
    return;
  }

  CameraComponent->SetRelativeTransform(FTransform(FRotator(-15.f, 0.f, 0.f), // Rotation
                                                   FVector(0.f, 0.f, 60.f),   // Translation
                                                   FVector(1.f, 1.f, 1.f)));  // Scale
}

void ABird::Move(const FInputActionValue& Value) {
  if (const auto& ForwardValue = Value.Get<float>(); Controller != nullptr && ForwardValue != 0.f) {
    DELTA_LOG("{}", DeltaFormat("Move: {}", Value.ToString()));

    const FVector& Forward = GetActorForwardVector();
    AddMovementInput(Forward, ForwardValue);
  }
}

void ABird::Rotate(const FInputActionValue& Value) {
  if (const auto& RotationValue = Value.Get<float>();
      Controller != nullptr && RotationValue != 0.f) {
    DELTA_LOG("{}", DeltaFormat("Rotate: {}", Value.ToString()));

    AddControllerYawInput(RotationValue);
  }
}

void ABird::Look(const FInputActionValue& Value) {
  if (const auto& LookValue = Value.Get<FVector2D>();
      Controller != nullptr && LookValue != FVector2D::ZeroVector) {
    DELTA_LOG("{}", DeltaFormat("Look: {}", Value.ToString()));

    AddControllerYawInput(LookValue.X);
    AddControllerPitchInput(LookValue.Y);
  }
}