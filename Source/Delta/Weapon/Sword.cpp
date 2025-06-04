// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Sword.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "../Interface/HitInterface.h"
#include "../Common/Finder.h"
#include "../Common/LogUtil.h"
#include "../Common/DebugShape.h"

ASword::ASword() {
  {
    static const TCHAR* const Path = TEXT("/Script/Engine.StaticMesh'/Game/Fab/Megascans/3D/Sword_uitlbiaga/Medium/"
                                          "SM_uitlbiaga_tier_2.SM_uitlbiaga_tier_2'");
    DELTA_SET_STATIC_MESH(StaticMeshComponent, Path);
  }

  {
    static const TCHAR* const Path = TEXT("/Script/MetasoundEngine.MetaSoundSource'/Game/Delta/MetaSound/sfx_Shink.sfx_Shink'");
    DELTA_SET_SOUNDBASE(EquipSound, Path);
  }

  {
    SphereComponent->SetSphereRadius(52.f);
  }

  {
    StaticMeshComponent->SetGenerateOverlapEvents(false);

    StaticMeshComponent->SetCollisionProfileName(TEXT("Custom"));
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
    StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
    StaticMeshComponent->UpdateCollisionProfile();
  }

  {
    WeaponBox->SetBoxExtent(FVector(2.029312, 1.120226, 40.305506));
    WeaponBox->SetRelativeLocation(FVector(0, 0, 11.5));

    WeaponBox->SetGenerateOverlapEvents(true);

    WeaponBox->SetCollisionProfileName(TEXT("Custom"));
    WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
    WeaponBox->UpdateCollisionProfile();
  }

  {
    BoxTraceStart->SetRelativeLocation(FVector(0.483424, -0.391544, 50.757500));
    BoxTraceEnd->SetRelativeLocation(FVector(0.000000, 0.468142, -27.463856));
  }

  {
    static const TCHAR* const Path = TEXT("/Script/Niagara.NiagaraSystem'/Game/Delta/Niagara/NS_Embers.NS_Embers'");
    DELTA_SET_NIAGARA_SYSTEM(EmbersEffect, Path);
  }
}

void ASword::Equip(USceneComponent* InParent, FName InSocketName) {
  Super::Equip(InParent, InSocketName);

  if (EquipSound == nullptr) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "EquipSound is null"));
    return;
  }

  if (SphereComponent == nullptr) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "SphereComponent is null"));
    return;
  }

  SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());

  if (EmbersEffect) {
    EmbersEffect->Deactivate();
  }
}

void ASword::OnWeaponBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                     AActor*              OtherActor,
                                     UPrimitiveComponent* OtherComp,
                                     int32                OtherBodyIndex,
                                     bool                 bFromSweep,
                                     const FHitResult&    SweepResult) {
  const FVector Start = BoxTraceStart->GetComponentLocation();
  const FVector End   = BoxTraceEnd->GetComponentLocation();

  TArray<TObjectPtr<AActor>> ActorsToIgnore;
  ActorsToIgnore.Add(this);

  for (AActor* Actor : IgnoreActors) {
    ActorsToIgnore.AddUnique(Actor);
  }

  FHitResult BoxHit;

  EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None;
#if DELTA_SWORD_ENABLE_DEBUG_HIT
  DrawDebugType = EDrawDebugTrace::ForDuration;
#endif

  UKismetSystemLibrary::BoxTraceSingle(this,                                  // WorldContextObject
                                       Start,                                 //
                                       End,                                   //
                                       FVector(5.f, 5.f, 5.f),                // BoxExtent
                                       BoxTraceStart->GetComponentRotation(), // Orientation
                                       ETraceTypeQuery::TraceTypeQuery1,      //
                                       false,                                 // bTraceComplex
                                       ActorsToIgnore,                        //
                                       DrawDebugType,                         //
                                       BoxHit,                                //
                                       true);                                 // bIgnoreSelf

  if (BoxHit.GetActor()) {
    IHitInterface* const HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
    if (HitInterface) {
      HitInterface->GetHit(BoxHit.ImpactPoint);
    }
    IgnoreActors.AddUnique(BoxHit.GetActor());
    CreateAttackFields(BoxHit.ImpactPoint);
  }
}

void ASword::CreateAttackFields(const FVector& FieldLocation) {
#if DELTA_SWORD_ENABLE_BLUEPRINT_ATTACK_FIELDS
  CreateBP_AttackFields(FieldLocation);
#else
  CreateNative_AttackFields(FieldLocation);
#endif
}

void ASword::CreateNative_AttackFields(const FVector& ImpactPoint) {
  ApplyRadialFalloffField(1000000.0f,
                          0.8f,
                          1.0f,
                          0.0f,
                          200.0f,
                          ImpactPoint,
                          EFieldFalloffType::Field_FallOff_None,
                          true,
                          EFieldPhysicsType::Field_ExternalClusterStrain,
                          nullptr);

  ApplyRadialVectorField(150000.0f, //
                         ImpactPoint,
                         true,
                         EFieldPhysicsType::Field_LinearForce,
                         FieldSystemMetaDataFilterComponent);

#if DELTA_SWORD_ENABLE_DEBUG_HIT
  DELTA_LOG("{}", DeltaFormat("[{}] Applied radial vector field at impact point: {}", DELTA_FUNCSIG, ImpactPoint.ToString()));
  DELTA_DEBUG_SPHERE(ImpactPoint);
#endif
}
