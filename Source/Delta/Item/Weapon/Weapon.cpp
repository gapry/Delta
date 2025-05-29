// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Weapon.h"
#include "Sound/SoundBase.h"
#include "Components/BoxComponent.h"
#include "../../Character/EchoCharacter.h"
#include "../../Common/LogUtil.h"

AWeapon::AWeapon() {
  WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
  WeaponBox->SetupAttachment(GetRootComponent());
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

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                 AActor*              OtherActor,
                                 UPrimitiveComponent* OtherComp,
                                 int32                OtherBodyIndex) {
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