// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "EchoCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Animation/AnimMontage.h"
#include "../Common/Finder.h"
#include "../Common/LogUtil.h"
#include "../Item/Sword.h"

AEchoCharacter::AEchoCharacter() {
  {
    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw   = false;
    bUseControllerRotationRoll  = false;
  }

  {
    SkeletalMeshComponent = GetMesh();

    static constexpr const TCHAR* const SkeletalMeshPath{TEXT(
      "/Script/Engine.SkeletalMesh'/Game/Delta/AncientContent/Characters/Echo/Meshes/Echo.Echo'")};

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

    static constexpr const TCHAR* const JumpActionPath{TEXT(
      "/Script/EnhancedInput.InputAction'/Game/Delta/Character/Input/IA_Echo_Jump.IA_Echo_Jump'")};
    DELTA_SET_InputAction(JumpAction, JumpActionPath);

    static constexpr const TCHAR* const EquipActionPath{
      TEXT("/Script/EnhancedInput.InputAction'/Game/Delta/Character/Input/"
           "IA_Echo_Equip.IA_Echo_Equip'")};
    DELTA_SET_InputAction(EquipAction, EquipActionPath);

    static constexpr const TCHAR* const AttackActionPath{
      TEXT("/Script/EnhancedInput.InputAction'/Game/Delta/Character/Input/"
           "IA_Echo_Attack.IA_Echo_Attack'")};
    DELTA_SET_InputAction(AttackAction, AttackActionPath);
  }

  {
    static constexpr const TCHAR* const HairPath{
      TEXT("/Script/HairStrandsCore.GroomAsset'/Game/AncientContent/Characters/Echo/Hair/"
           "Hair_S_UpdoBuns.Hair_S_UpdoBuns'")};
    static constexpr const TCHAR* const MaterialPath{
      TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Delta/Character/Hair/"
           "MI_HairGroom.MI_HairGroom'")};

    static constexpr const TCHAR* const ComponentName{TEXT("Hair")};
    static const FString                SocketName = FString("head");

    HairComponent = CreateDefaultSubobject<UGroomComponent>(ComponentName);
    HairComponent->SetupAttachment(SkeletalMeshComponent.Get());
    HairComponent->AttachmentName = SocketName;

    DELTA_SET_GROOM(HairComponent, HairPath);
    DELTA_SET_GROOM_MATERIAL(HairComponent, 0, MaterialPath);
  }

  {
    static constexpr const TCHAR* const EyebrowsPath{
      TEXT("/Script/HairStrandsCore.GroomAsset'/Game/AncientContent/Characters/Echo/Hair/"
           "Eyebrows_L_Echo.Eyebrows_L_Echo'")};
    static constexpr const TCHAR* const ComponentName{TEXT("Eyebrows")};
    static const FString                SocketName = FString("head");

    EyebrowsComponent = CreateDefaultSubobject<UGroomComponent>(ComponentName);
    EyebrowsComponent->SetupAttachment(SkeletalMeshComponent.Get());
    EyebrowsComponent->AttachmentName = SocketName;
    DELTA_SET_GROOM(EyebrowsComponent, EyebrowsPath);
  }

  {
    static constexpr const TCHAR* const AnimBlueprintPath{
      TEXT("/Script/Engine.AnimBlueprint'/Game/Delta/Character/Animation/Blueprint/"
           "ABP_EchoCharacter.ABP_EchoCharacter_C'")};
    DELTA_SET_ANIMATION_BLUEPRINT(SkeletalMeshComponent.Get(), AnimBlueprintPath);
  }

  {
    static constexpr const TCHAR* const AnimMontagePath{TEXT(
      "/Script/Engine.AnimMontage'/Game/Delta/Character/Animation/Montage/AM_Attack.AM_Attack'")};
    DELTA_SET_ANIMATION_MONTAGE(AttackMontage, AnimMontagePath);
  }

  {
    SkeletalMeshComponent->SetCollisionProfileName(TEXT("Custom"));
    SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SkeletalMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,
                                                         ECollisionResponse::ECR_Ignore);
    SkeletalMeshComponent->UpdateCollisionProfile();
  }
}

void AEchoCharacter::PostInitializeComponents() {
  Super::PostInitializeComponents();

  PostInitializeSkeletalMeshComponent();
  PostInitializeCapsuleComponent();
  PostInitializeSpringArmComponent();
  PostInitializeCameraComponent();
  PostInitializeCharacterMovementComponent();
}

void AEchoCharacter::PostInitializeSkeletalMeshComponent() {
  if (!SkeletalMeshComponent.IsValid()) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "SkeletalMeshComponent is not valid"));
  }

  SkeletalMeshComponent->SetRelativeTransform(FTransform(FRotator(0.f, -90.f, 0.f), // Rotation
                                                         FVector(0.f, 0.f, -90.f),  // Translation
                                                         FVector(1.f, 1.f, 1.f)));  // Scale

  SkeletalMeshComponent->SetGenerateOverlapEvents(true);
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

void AEchoCharacter::PostInitializeCharacterMovementComponent() {
  auto* const Movement = GetCharacterMovement();
  if (!Movement) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "CharacterMovement is null"));
  }

  Movement->bOrientRotationToMovement  = true;
  Movement->RotationRate               = FRotator(0.0f, 500.0f, 0.0f);
  Movement->JumpZVelocity              = 700.f;
  Movement->AirControl                 = 0.35f;
  Movement->MaxWalkSpeed               = 500.f;
  Movement->MinAnalogWalkSpeed         = 20.f;
  Movement->BrakingDecelerationWalking = 2000.f;
  Movement->BrakingDecelerationFalling = 1500.0f;
}

void AEchoCharacter::BeginPlay() {
  Super::BeginPlay();

  auto* const Subsystem = GetSubsystem();
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

void AEchoCharacter::NotifyControllerChanged() {
  Super::NotifyControllerChanged();
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

  if (!JumpAction) {
    DELTA_LOG("{}", DeltaFormat("JumpAction is null"));
    return;
  }

  if (!EquipAction) {
    DELTA_LOG("{}", DeltaFormat("EquipAction is null"));
    return;
  }

  if (!AttackAction) {
    DELTA_LOG("{}", DeltaFormat("AttackAction is null"));
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

  EnhancedInputComponent->BindAction(JumpAction,
                                     ETriggerEvent::Triggered,
                                     this,
                                     &AEchoCharacter::Jump);

  EnhancedInputComponent->BindAction(EquipAction,
                                     ETriggerEvent::Triggered,
                                     this,
                                     &AEchoCharacter::Equip);

  EnhancedInputComponent->BindAction(AttackAction,
                                     ETriggerEvent::Triggered,
                                     this,
                                     &AEchoCharacter::Attack);
}

void AEchoCharacter::Move(const FInputActionValue& Value) {
  if (Controller == nullptr) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "Controller is null"));
    return;
  }

  if (ActionState == EActionState::EAS_Attacking) {
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

void AEchoCharacter::Jump() {
  Super::Jump();
}

void AEchoCharacter::Equip(const FInputActionValue& Value) {
  auto* const OverlappingWeapon = Cast<ASword>(OverlappingItem);

  if (OverlappingWeapon == nullptr) {
    return;
  }

  OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"));
  CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
}

void AEchoCharacter::Attack(const FInputActionValue& Value) {
  if (Controller == nullptr) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "Controller is null"));
    return;
  }

  if (!CanAttack()) {
    return;
  }

  if (!Value.Get<bool>()) {
    return;
  }

  PlayAttackMontage();
  ActionState = EActionState::EAS_Attacking;
}

APlayerController* AEchoCharacter::GetPlayerController() const {
  return CastChecked<APlayerController>(Controller);
}

UEnhancedInputLocalPlayerSubsystem* AEchoCharacter::GetSubsystem() const {
  auto* const PC = GetPlayerController();

  if (PC == nullptr) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "PlayerController is null"));
    return nullptr;
  }

  return ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
}

void AEchoCharacter::SetOverlappingItem(AItem* const Item) {
  if (Item == nullptr) {
    OverlappingItem = nullptr;
    return;
  }

  if (OverlappingItem.Get() == Item) {
    return;
  }

  OverlappingItem = Item;
}

AItem* AEchoCharacter::GetOverlappingItem() const {
  return OverlappingItem.Get();
}

ECharacterState AEchoCharacter::GetCharacterState() const {
  return CharacterState;
}

void AEchoCharacter::SetCharacterState(ECharacterState NewState) {
  if (CharacterState == NewState) {
    return;
  }
  CharacterState = NewState;
}

void AEchoCharacter::PlayAttackMontage() const {
  UAnimInstance* const AnimInstance = SkeletalMeshComponent->GetAnimInstance();

  if (!AnimInstance) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "AnimInstance is null"));
    return;
  }

  if (!AttackMontage) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "AttackMontage is null"));
    return;
  }

  AnimInstance->Montage_Play(AttackMontage);

  const int32  Selection   = FMath::RandRange(0, 2);
  static FName SectionName = FName();
  switch (Selection) {
    case 0: SectionName = FName("Attack_Horizontal"); break;
    case 1: SectionName = FName("Attack_360"); break;
    case 2: SectionName = FName("Attack_Downward"); break;
    default: break;
  }
  AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
}

void AEchoCharacter::AttackAnimNotify() {
  ActionState = EActionState::EAS_Unoccupied;
}

bool AEchoCharacter::CanAttack() const {
  return ActionState == EActionState::EAS_Unoccupied &&
         CharacterState != ECharacterState::ECS_Unequipped;
}