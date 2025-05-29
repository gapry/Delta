// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Sword.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../Common/Finder.h"
#include "../../Common/LogUtil.h"

ASword::ASword() {
  {
    static const TCHAR* const Path =
      TEXT("/Script/Engine.StaticMesh'/Game/Fab/Megascans/3D/Sword_uitlbiaga/Medium/"
           "SM_uitlbiaga_tier_2.SM_uitlbiaga_tier_2'");
    DELTA_SET_STATIC_MESH(StaticMeshComponent, Path);
  }

  {
    static const TCHAR* const Path =
      TEXT("/Script/MetasoundEngine.MetaSoundSource'/Game/Delta/MetaSound/sfx_Shink.sfx_Shink'");
    DELTA_SET_SOUNDBASE(EquipSound, Path);
  }

  {
    SphereComponent->SetSphereRadius(52.f);
  }

  {
    StaticMeshComponent->SetGenerateOverlapEvents(false);

    StaticMeshComponent->SetCollisionProfileName(TEXT("Custom"));
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    StaticMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,
                                                       ECollisionResponse::ECR_Ignore);
    StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,
                                                       ECollisionResponse::ECR_Overlap);
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
    WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,
                                             ECollisionResponse::ECR_Ignore);
    WeaponBox->UpdateCollisionProfile();
  }

  {
    BoxTraceStart->SetRelativeLocation(FVector(0.000000, 0.468142, -27.463856));
    BoxTraceEnd->SetRelativeLocation(FVector(0.483424, -0.391544, 50.757500));
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

  FHitResult BoxHit;

  UKismetSystemLibrary::BoxTraceSingle(this,                                  // WorldContextObject
                                       Start,                                 //
                                       End,                                   //
                                       FVector(5.f, 5.f, 5.f),                // BoxExtent
                                       BoxTraceStart->GetComponentRotation(), // Orientation
                                       ETraceTypeQuery::TraceTypeQuery1,      //
                                       false,                                 // bTraceComplex
                                       ActorsToIgnore,                        //
                                       EDrawDebugTrace::ForDuration,          //
                                       BoxHit,                                //
                                       true);                                 // bIgnoreSelf
}
