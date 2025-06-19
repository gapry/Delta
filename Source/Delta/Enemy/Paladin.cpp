// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Paladin.h"
#include "Engine/EngineTypes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "../Common/Finder.h"
#include "../Component/HealthBarComponent.h"
#include "../Weapon/Sword.h"

APaladin::APaladin() {
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
      TEXT("/Script/Engine.AnimBlueprint'/Game/Delta/Enemy/Paladin/Animation/Blueprint/ABP_Enemy.ABP_Enemy_C'")};
    DELTA_SET_ANIMATION_BLUEPRINT(SkeletalMeshComponent.Get(), AnimBlueprintPath);
  }

  {
    static constexpr const TCHAR* const MontagePath{
      TEXT("/Script/Engine.AnimMontage'/Game/Delta/Enemy/Paladin/Animation/Montage/AM_HitReact.AM_HitReact'")};
    DELTA_SET_ANIMATION_MONTAGE(HitReactMontage, MontagePath);
  }

  {
    static constexpr const TCHAR* const MontagePath{
      TEXT("/Script/Engine.AnimMontage'/Game/Delta/Enemy/Paladin/Animation/Montage/AM_Death.AM_Death'")};
    DELTA_SET_ANIMATION_MONTAGE(DeathMontage, MontagePath);
  }

  {
    static constexpr const TCHAR* const MontagePath{
      TEXT("/Script/Engine.AnimMontage'/Game/Delta/Enemy/Paladin/Animation/Montage/AM_Attack.AM_Attack'")};
    DELTA_SET_ANIMATION_MONTAGE(AttackMontage, MontagePath);
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
    Shield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));

    static constexpr const TCHAR* const StaticMeshPath{TEXT("/Script/Engine.StaticMesh'/Game/AncientTreasures/Meshes/SM_Shield_01a.SM_Shield_01a'")};
    DELTA_SET_STATIC_MESH(Shield, StaticMeshPath);

    Shield->SetCollisionProfileName(TEXT("Custom"));
    Shield->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Shield->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    Shield->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    Shield->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
    Shield->UpdateCollisionProfile();
  }
}

void APaladin::PostInitializeComponents() {
  Super::PostInitializeComponents();

  FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
  Shield->AttachToComponent(SkeletalMeshComponent.Get(), TransformRules, TEXT("LeftHandSocket"));

  WeaponClass = ASword::StaticClass();
  if (UWorld* World = GetWorld(); World && WeaponClass) {
    auto* const DefaultWeapon = World->SpawnActor<ASword>(WeaponClass);
    DefaultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
    EquippedWeapon = DefaultWeapon;
  }
}

void APaladin::PlayAttackMontage() {
  Super::PlayAttackMontage();

  UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
  if (AnimInstance && AttackMontage) {
    AnimInstance->Montage_Play(AttackMontage);
    TArray<FName> AttackSectionNames;
    GetAllMontageSectionsNames(AttackMontage, AttackSectionNames);
    if (!AttackSectionNames.IsEmpty()) {
      const int32 RandomIndex = FMath::RandRange(0, AttackSectionNames.Num() - 1);
      const FName SectionName = AttackSectionNames[RandomIndex];
      AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
    }
  }
}
