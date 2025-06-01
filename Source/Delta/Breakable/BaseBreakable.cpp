// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "BaseBreakable.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/AssertionMacros.h"
#include "../Common/LogUtil.h"

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
  GeometryCollectionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
  GeometryCollectionComponent->UpdateCollisionProfile();

  RootComponent = GeometryCollectionComponent;
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
}

void ABaseBreakable::OnBreakEvent(const FChaosBreakEvent& BreakEvent) {
  SetLifeSpan(LifeSpan);
}