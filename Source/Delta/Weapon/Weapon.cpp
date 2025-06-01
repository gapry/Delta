// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Weapon.h"
#include "Sound/SoundBase.h"
#include "Components/BoxComponent.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemObjects.h"
#include "../Player/EchoCharacter.h"
#include "../Common/LogUtil.h"
#include "../Common/DebugShape.h"

AWeapon::AWeapon() {
  {
    WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
    WeaponBox->SetupAttachment(GetRootComponent());
  }

  {
    BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
    BoxTraceStart->SetupAttachment(GetRootComponent());

    BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
    BoxTraceEnd->SetupAttachment(GetRootComponent());
  }

  {
    FieldSystemComponent = CreateDefaultSubobject<UFieldSystemComponent>(TEXT("FieldSystemComponent"));
    check(FieldSystemComponent);

    RadialFalloffComponent = CreateDefaultSubobject<URadialFalloff>(TEXT("RadialFalloffComponent"));
    check(RadialFalloffComponent);

    RadialVectorComponent = CreateDefaultSubobject<URadialVector>(TEXT("RadialVectorComponent"));
    check(RadialVectorComponent);

    FieldSystemMetaDataFilterComponent = CreateDefaultSubobject<UFieldSystemMetaDataFilter>(TEXT("FieldSystemMetaDataFilterComponent"));
    check(FieldSystemMetaDataFilterComponent);

    FieldSystemMetaDataFilterComponent->FilterType   = EFieldFilterType::Field_Filter_Dynamic;
    FieldSystemMetaDataFilterComponent->ObjectType   = EFieldObjectType::Field_Object_Destruction;
    FieldSystemMetaDataFilterComponent->PositionType = EFieldPositionType::Field_Position_CenterOfMass;
  }
}

void AWeapon::BeginPlay() {
  Super::BeginPlay();

  WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponBoxBeginOverlap);
}

void AWeapon::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  if (ItemState != EItemState::EIS_Hovering) {
    return;
  }

  const float DeltaZ = GetSineOscillationOffset();

  FVector MoveLocation = GetActorLocation();
  MoveLocation.Z += DeltaZ;
  SetActorLocation(MoveLocation);

  FRotator MoveRotation = GetActorRotation();
  MoveRotation.Yaw += RotationRate * DeltaTime;
  SetActorRotation(MoveRotation);
}

void AWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                   AActor*              OtherActor,
                                   UPrimitiveComponent* OtherComp,
                                   int32                OtherBodyIndex,
                                   bool                 bFromSweep,
                                   const FHitResult&    SweepResult) {
  auto* const EchoCharacter = Cast<AEchoCharacter>(OtherActor);
  if (EchoCharacter == nullptr) {
    return;
  }
  EchoCharacter->SetOverlappingItem(this);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
  auto* const EchoCharacter = Cast<AEchoCharacter>(OtherActor);
  if (EchoCharacter == nullptr) {
    return;
  }
  EchoCharacter->SetOverlappingItem(nullptr);
}

void AWeapon::OnWeaponBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                      AActor*              OtherActor,
                                      UPrimitiveComponent* OtherComp,
                                      int32                OtherBodyIndex,
                                      bool                 bFromSweep,
                                      const FHitResult&    SweepResult) {
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName) {
  FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
  StaticMeshComponent->AttachToComponent(InParent, TransformRules, InSocketName);
  ItemState = EItemState::EIS_Equipped;
}

void AWeapon::AttackMeshToSocket(USceneComponent* const InParent, const FName InSocketName) const {
  const FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
  StaticMeshComponent->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AWeapon::DebugOverlap(AActor* OtherActor) {
  if (OtherActor != nullptr) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, OtherActor->GetName()));

    DrawDebugSphere(GetWorld(), OtherActor->GetActorLocation(), 50.f, 12, FColor::Red, false, 5.f);
  }
}

UBoxComponent* AWeapon::GetWeaponBox() const {
  return WeaponBox;
}

void AWeapon::ApplyRadialFalloffField(float                 Magnitude,
                                      float                 MinRange,
                                      float                 MaxRange,
                                      float                 Default,
                                      float                 Radius,
                                      const FVector&        Position,
                                      EFieldFalloffType     FalloffType,
                                      bool                  bEnabled,
                                      EFieldPhysicsType     PhysicsType,
                                      UFieldSystemMetaData* MetaData) {
  auto* const FieldNode = RadialFalloffComponent->SetRadialFalloff(Magnitude, //
                                                                   MinRange,
                                                                   MaxRange,
                                                                   Default,
                                                                   Radius,
                                                                   Position,
                                                                   FalloffType);

  FieldSystemComponent->ApplyPhysicsField(bEnabled, PhysicsType, MetaData, FieldNode);
}

void AWeapon::ApplyRadialVectorField(float                 Magnitude,
                                     const FVector&        Position,
                                     bool                  bEnabled,
                                     EFieldPhysicsType     PhysicsType,
                                     UFieldSystemMetaData* MetaData) {
  auto* const FieldNode = RadialVectorComponent->SetRadialVector(Magnitude, Position);
  FieldSystemComponent->ApplyPhysicsField(bEnabled, PhysicsType, MetaData, FieldNode);
}

void AWeapon::CreateAttackFields(const FVector& ImpactPoint) {
}
