// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "EchoCharacter.h"

#include "../Common/Finder.h"
#include "../Common/LogUtil.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AEchoCharacter::AEchoCharacter() {
  {
    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw   = false;
    bUseControllerRotationRoll  = false;
  }

  {
    SkeletalMeshComponent = GetMesh();

    static constexpr const TCHAR* const SkeletalMeshPath{
      TEXT("/Script/Engine.SkeletalMesh'/Game/AncientContent/Characters/Echo/Meshes/Echo.Echo'")};

    DELTA_SET_SKELETAL_MESH(SkeletalMeshComponent.Get(), SkeletalMeshPath);
  }

  {
    CapsuleComponent = GetCapsuleComponent();
  }

  {
    static constexpr const TCHAR* const ComponentName{TEXT("SpringArm")};
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(ComponentName);
    SpringArmComponent->SetupAttachment(GetRootComponent());
  }

  {
    static constexpr const TCHAR* const ComponentName{TEXT("Camera")};
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(ComponentName);
    CameraComponent->SetupAttachment(SpringArmComponent.Get(), USpringArmComponent::SocketName);
  }

  {
    static constexpr const TCHAR* const InputMappingContextPath{TEXT(
      "/Script/EnhancedInput.InputMappingContext'/Game/Delta/Character/Input/IMC_Echo.IMC_Echo'")};
    DELTA_SET_InputMappingContext(InputMappingContext, InputMappingContextPath);

    static constexpr const TCHAR* const MoveActionPath{TEXT(
      "/Script/EnhancedInput.InputAction'/Game/Delta/Character/Input/IA_Echo_Move.IA_Echo_Move'")};
    DELTA_SET_InputAction(MoveAction, MoveActionPath);

    static constexpr const TCHAR* const LookActionPath{TEXT(
      "/Script/EnhancedInput.InputAction'/Game/Delta/Character/Input/IA_Echo_Look.IA_Echo_Look'")};
    DELTA_SET_InputAction(LookAction, LookActionPath);
  }
}

void AEchoCharacter::PostInitializeComponents() {
  Super::PostInitializeComponents();

  PostInitializeSkeletalMeshComponent();
  PostInitializeCapsuleComponent();
  PostInitializeSpringArmComponent();
  PostInitializeCameraComponent();
}

void AEchoCharacter::PostInitializeSkeletalMeshComponent() {
  if (!SkeletalMeshComponent.IsValid()) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "SkeletalMeshComponent is not valid"));
  }

  SkeletalMeshComponent->SetRelativeTransform(FTransform(FRotator(0.f, -90.f, 0.f), // Rotation
                                                         FVector(0.f, 0.f, -90.f),  // Translation
                                                         FVector(1.f, 1.f, 1.f)));  // Scale
}

void AEchoCharacter::PostInitializeCapsuleComponent() {
  if (!CapsuleComponent.IsValid()) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "CapsuleComponent is not valid"));
  }

  CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
}

void AEchoCharacter::PostInitializeSpringArmComponent() {
  if (!SpringArmComponent.IsValid()) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "SpringArmComponent is not valid"));
  }

  SpringArmComponent->bUsePawnControlRotation = true;
  SpringArmComponent->TargetArmLength         = 300.f;
}

void AEchoCharacter::PostInitializeCameraComponent() {
  if (!CameraComponent.IsValid()) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "CameraComponent is not valid"));
  }

  CameraComponent->bUsePawnControlRotation = false;
  CameraComponent->SetRelativeTransform(FTransform(FRotator(-15.f, 0.f, 0.f), //
                                                   FVector(0.f, 0.f, 60.f),   //
                                                   FVector(1.f, 1.f, 1.f)));  //
}

void AEchoCharacter::BeginPlay() {
  Super::BeginPlay();

  auto* const Subsystem = GetSubsytem();
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

void AEchoCharacter::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
}

void AEchoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  auto* const EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

  if (!MoveAction) {
    DELTA_LOG("{}", DeltaFormat("MoveAction is null"));
    return;
  }

  if (!LookAction) {
    DELTA_LOG("{}", DeltaFormat("LookAction is null"));
    return;
  }

  EnhancedInputComponent->BindAction(MoveAction,
                                     ETriggerEvent::Triggered,
                                     this,
                                     &AEchoCharacter::Move);

  EnhancedInputComponent->BindAction(LookAction,
                                     ETriggerEvent::Triggered,
                                     this,
                                     &AEchoCharacter::Look);
}

void AEchoCharacter::Move(const FInputActionValue& Value) {

  if (Controller == nullptr) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "Controller is null"));
    return;
  }

  const FVector2D& MovementVector = Value.Get<FVector2D>();
  if (MovementVector.IsZero()) {
    return;
  }

  const FRotator& Rotation = Controller->GetControlRotation();

  const FRotator YawRotation(0, Rotation.Yaw, 0);

  const FVector& ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
  const FVector& RightDirection   = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

  AddMovementInput(ForwardDirection, MovementVector.Y);
  AddMovementInput(RightDirection, MovementVector.X);
}

void AEchoCharacter::Look(const FInputActionValue& Value) {
  if (Controller == nullptr) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "Controller is null"));
    return;
  }

  const FVector2D& LookAxisVector = Value.Get<FVector2D>();
  if (LookAxisVector.IsZero()) {
    return;
  }

  AddControllerYawInput(LookAxisVector.X);
  AddControllerPitchInput(LookAxisVector.Y);
}

APlayerController* AEchoCharacter::GetPlayerController() const {
  return CastChecked<APlayerController>(Controller);
}

UEnhancedInputLocalPlayerSubsystem* AEchoCharacter::GetSubsytem() const {
  if (auto* const PC = GetPlayerController()) {
    return ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
  }
  return nullptr;
}
