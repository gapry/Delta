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
#include "../../Common/Finder.h"
#include "../../Common/LogUtil.h"
#include "../../Weapon/Weapon.h"
#include "../../Weapon/Sword.h"

AEchoCharacter::AEchoCharacter() {
  {
    SkeletalMeshComponent = GetMesh();

    static constexpr const TCHAR* const SkeletalMeshPath{
      TEXT("/Script/Engine.SkeletalMesh'/Game/Delta/AncientContent/Characters/Echo/Meshes/Echo.Echo'")};

    DELTA_SET_SKELETAL_MESH(SkeletalMeshComponent.Get(), SkeletalMeshPath);

    SkeletalMeshComponent->SetRelativeTransform(FTransform(FRotator(0.f, -90.f, 0.f), // Rotation
                                                           FVector(0.f, 0.f, -90.f),  // Translation
                                                           FVector(1.f, 1.f, 1.f)));  // Scale

    SkeletalMeshComponent->SetGenerateOverlapEvents(true);

    SkeletalMeshComponent->SetCollisionProfileName(TEXT("Custom"));
    SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SkeletalMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
    SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
    SkeletalMeshComponent->UpdateCollisionProfile();
  }

  {
    CapsuleComponent = GetCapsuleComponent();

    CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
    CapsuleComponent->SetGenerateOverlapEvents(true);

    CapsuleComponent->SetCollisionProfileName(TEXT("Custom"));
    CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
    CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
    CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    CapsuleComponent->UpdateCollisionProfile();
  }

  {
    static constexpr const TCHAR* const ComponentName{TEXT("SpringArm")};
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(ComponentName);
    SpringArmComponent->SetupAttachment(GetRootComponent());

    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->TargetArmLength         = 300.f;
  }

  {
    static constexpr const TCHAR* const ComponentName{TEXT("Camera")};
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(ComponentName);
    CameraComponent->SetupAttachment(SpringArmComponent.Get(), USpringArmComponent::SocketName);
    CameraComponent->bUsePawnControlRotation = false;
    CameraComponent->SetRelativeTransform(FTransform(FRotator(-15.f, 0.f, 0.f), //
                                                     FVector(0.f, 0.f, 60.f),   //
                                                     FVector(1.f, 1.f, 1.f)));  //
  }

  {
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

  {
    static constexpr const TCHAR* const InputMappingContextPath{
      TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Delta/Character/Input/IMC_Echo.IMC_Echo'")};
    DELTA_SET_InputMappingContext(InputMappingContext, InputMappingContextPath);

    static constexpr const TCHAR* const MoveActionPath{
      TEXT("/Script/EnhancedInput.InputAction'/Game/Delta/Character/Input/IA_Echo_Move.IA_Echo_Move'")};
    DELTA_SET_InputAction(MoveAction, MoveActionPath);

    static constexpr const TCHAR* const LookActionPath{
      TEXT("/Script/EnhancedInput.InputAction'/Game/Delta/Character/Input/IA_Echo_Look.IA_Echo_Look'")};
    DELTA_SET_InputAction(LookAction, LookActionPath);

    static constexpr const TCHAR* const JumpActionPath{
      TEXT("/Script/EnhancedInput.InputAction'/Game/Delta/Character/Input/IA_Echo_Jump.IA_Echo_Jump'")};
    DELTA_SET_InputAction(JumpAction, JumpActionPath);

    static constexpr const TCHAR* const EquipActionPath{TEXT("/Script/EnhancedInput.InputAction'/Game/Delta/Character/Input/"
                                                             "IA_Echo_Equip.IA_Echo_Equip'")};
    DELTA_SET_InputAction(EquipAction, EquipActionPath);

    static constexpr const TCHAR* const AttackActionPath{TEXT("/Script/EnhancedInput.InputAction'/Game/Delta/Character/Input/"
                                                              "IA_Echo_Attack.IA_Echo_Attack'")};
    DELTA_SET_InputAction(AttackAction, AttackActionPath);
  }

  {
    static constexpr const TCHAR* const HairPath{TEXT("/Script/HairStrandsCore.GroomAsset'/Game/AncientContent/Characters/Echo/Hair/"
                                                      "Hair_S_UpdoBuns.Hair_S_UpdoBuns'")};
    static constexpr const TCHAR* const MaterialPath{TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Delta/Character/Hair/"
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
    static constexpr const TCHAR* const EyebrowsPath{TEXT("/Script/HairStrandsCore.GroomAsset'/Game/AncientContent/Characters/Echo/Hair/"
                                                          "Eyebrows_L_Echo.Eyebrows_L_Echo'")};
    static constexpr const TCHAR* const ComponentName{TEXT("Eyebrows")};
    static const FString                SocketName = FString("head");

    EyebrowsComponent = CreateDefaultSubobject<UGroomComponent>(ComponentName);
    EyebrowsComponent->SetupAttachment(SkeletalMeshComponent.Get());
    EyebrowsComponent->AttachmentName = SocketName;
    DELTA_SET_GROOM(EyebrowsComponent, EyebrowsPath);
  }

  {
    static constexpr const TCHAR* const AnimBlueprintPath{TEXT("/Script/Engine.AnimBlueprint'/Game/Delta/Character/Animation/Blueprint/"
                                                               "ABP_EchoCharacter.ABP_EchoCharacter_C'")};
    DELTA_SET_ANIMATION_BLUEPRINT(SkeletalMeshComponent.Get(), AnimBlueprintPath);
  }

  {
    static constexpr const TCHAR* const MontagePath{TEXT("/Script/Engine.AnimMontage'/Game/Delta/Character/Animation/Montage/AM_Attack.AM_Attack'")};
    DELTA_SET_ANIMATION_MONTAGE(AttackMontage, MontagePath);
  }

  {
    static constexpr const TCHAR* const MontagePath{TEXT("/Script/Engine.AnimMontage'/Game/Delta/Character/Animation/Montage/"
                                                         "AM_EquipUnequip.AM_EquipUnequip'")};
    DELTA_SET_ANIMATION_MONTAGE(EquipUnequipMontage, MontagePath);
  }

  {
    Tags.Add(FName("EchoCharacter"));
  }
}

void AEchoCharacter::BeginPlay() {
  Super::BeginPlay();

  if (const auto* const PlayerController = Cast<APlayerController>(Controller)) {
    if (auto* const Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
      Subsystem->AddMappingContext(InputMappingContext, 0);
    }
  }
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

  EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEchoCharacter::Move);
  EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEchoCharacter::Look);
  EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AEchoCharacter::Jump);
  EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &AEchoCharacter::Equip);
  EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AEchoCharacter::Attack);
}

void AEchoCharacter::Move(const FInputActionValue& Value) {
  if (Controller == nullptr) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "Controller is null"));
    return;
  }

  if (ActionState == EActionState::EAS_Attacking || ActionState == EActionState::EAS_EquippingWeapon) {
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
    if (CanDisarm()) {
      PlayEquipUnequipMontage(FName("Unequip"));
      CharacterState = ECharacterState::ECS_Unequipped;
      ActionState    = EActionState::EAS_EquippingWeapon;
      return;
    }

    if (CanArm()) {
      PlayEquipUnequipMontage(FName("Equip"));
      CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
      ActionState    = EActionState::EAS_EquippingWeapon;
      return;
    }
    return;
  }

  OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
  CharacterState  = ECharacterState::ECS_EquippedOneHandedWeapon;
  EquippedWeapon  = OverlappingWeapon;
  OverlappingItem = nullptr;
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

void AEchoCharacter::PlayAttackMontage() {
  UAnimInstance* const AnimInstance = SkeletalMeshComponent->GetAnimInstance();

  if (!AnimInstance) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "AnimInstance is null"));
    return;
  }

  if (!AttackMontage) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "AttackMontage is null"));
    return;
  }

  const float DefaultPlayRate = AttackMontage->RateScale;
  const float NewPlayRate     = DefaultPlayRate * 1.5f;

  AnimInstance->Montage_Play(AttackMontage, NewPlayRate);

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

void AEchoCharacter::PlayEquipUnequipMontage(const FName SectionName) const {
  UAnimInstance* const AnimInstance = SkeletalMeshComponent->GetAnimInstance();

  if (!AnimInstance) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "AnimInstance is null"));
    return;
  }

  if (!EquipUnequipMontage) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "EquipUnequipMontage is null"));
    return;
  }

  AnimInstance->Montage_Play(EquipUnequipMontage);
  AnimInstance->Montage_JumpToSection(SectionName, EquipUnequipMontage);
}

void AEchoCharacter::AttackAnimNotify() {
  ActionState = EActionState::EAS_Unoccupied;
}

bool AEchoCharacter::CanArm() const {
  return ActionState == EActionState::EAS_Unoccupied &&       //
         CharacterState == ECharacterState::ECS_Unequipped && //
         EquippedWeapon != nullptr;                           //
}

bool AEchoCharacter::CanDisarm() const {
  return ActionState == EActionState::EAS_Unoccupied &&       //
         CharacterState != ECharacterState::ECS_Unequipped && //
         EquipUnequipMontage != nullptr;                      //
}

bool AEchoCharacter::CanAttack() {
  return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
}

void AEchoCharacter::Die() {
}

void AEchoCharacter::Arm() {
  if (!EquippedWeapon) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "EquippedWeapon is null"));
    return;
  }
  EquippedWeapon->AttackMeshToSocket(GetMesh(), FName("RightHandSocket"));
}

void AEchoCharacter::Disarm() {
  if (!EquippedWeapon) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "EquippedWeapon is null"));
    return;
  }
  EquippedWeapon->AttackMeshToSocket(GetMesh(), FName("SpineSocket"));
}

void AEchoCharacter::FinishEquipping() {
  ActionState = EActionState::EAS_Unoccupied;
}

void AEchoCharacter::GetHit(const FVector& ImpactPoint) {
}
