// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "BaseBreakable.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/AssertionMacros.h"
#include "../Treasure/BaseTreasure.h"
#include "../Common/LogUtil.h"
#include "../Common/Finder.h"

ABaseBreakable::ABaseBreakable() {
  PrimaryActorTick.bCanEverTick = true;

  GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
  check(GeometryCollectionComponent);

  GeometryCollectionComponent->bUseSizeSpecificDamageThreshold = false;
  GeometryCollectionComponent->SetGenerateOverlapEvents(true);
  GeometryCollectionComponent->SetNotifyRigidBodyCollision(false);
  GeometryCollectionComponent->SetNotifyBreaks(true);

  GeometryCollectionComponent->SetCollisionProfileName(TEXT("Custom"));
  GeometryCollectionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
  GeometryCollectionComponent->SetCollisionObjectType(ECollisionChannel::ECC_Destructible);
  GeometryCollectionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  GeometryCollectionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
  GeometryCollectionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
  GeometryCollectionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
  GeometryCollectionComponent->UpdateCollisionProfile();

  RootComponent = GeometryCollectionComponent;

  Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
  Capsule->SetupAttachment(GetRootComponent());

  Capsule->SetCollisionProfileName(TEXT("Custom"));
  Capsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
  Capsule->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
  Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
  Capsule->UpdateCollisionProfile();

  static const TCHAR* const Path = TEXT("/Script/MetasoundEngine.MetaSoundSource'/Game/Delta/MetaSound/sfx_PotBreak.sfx_PotBreak'");
  DELTA_SET_SOUNDBASE(BreakSound, Path);
}

void ABaseBreakable::BeginPlay() {
  Super::BeginPlay();
  GeometryCollectionComponent->OnChaosBreakEvent.AddDynamic(this, &ABaseBreakable::OnBreakEvent);
}

void ABaseBreakable::GetHit(const FVector& ImpactPoint) {
#if DELTA_BREAKABLE_ENABLE_DEBUG_HIT
  DELTA_LOG("[{}] Impact point: {}", DELTA_FUNCSIG, ImpactPoint.ToString());
#endif
  UGameplayStatics::PlaySoundAtLocation(GetWorld(), BreakSound, ImpactPoint);

  if (UWorld* const World = GetWorld(); World != nullptr && TreasureClasses.Num() > 0) {
    FVector Location = GetActorLocation();
    Location.Z += 55.f;

    const int32 Selection = FMath::RandRange(0, TreasureClasses.Num() - 1);
    World->SpawnActor<ABaseTreasure>(TreasureClasses[Selection], Location, GetActorRotation());

    if (Capsule) {
      Capsule->DestroyComponent();
      Capsule = nullptr;
    }
  }
}

void ABaseBreakable::OnBreakEvent(const FChaosBreakEvent& BreakEvent) {
  SetLifeSpan(LifeSpan);
}

void ABaseBreakable::AddTreasureClass(TSubclassOf<ABaseTreasure> InsertedTreasureClass) {
  if (InsertedTreasureClass) {
    TreasureClasses.Add(InsertedTreasureClass);
  }
}