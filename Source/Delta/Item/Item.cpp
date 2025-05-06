// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Item.h"
#include "../Common/LogUtil.h"
#include "../Common/DebugShape.h"

AItem::AItem() {
  PrimaryActorTick.bCanEverTick = true;
  if (RootComponent)
    RootComponent->SetMobility(EComponentMobility::Movable);
}

void AItem::BeginPlay() {
  Super::BeginPlay();

  SetLocation(FVector(0.f, 0.f, 50.f));
  SetRotation(FRotator(0.f, 45.f, 0.f));
  SetForwardDirection(GetActorForwardVector().GetSafeNormal());

  RenderDebugShape();
}

void AItem::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
}

void AItem::RenderDebugShape() const {
  DELTA_DEBUG_ARROW(CurrentLocation, ForwardDirection * 100.f);
  DELTA_DEBUG_SPHERE(CurrentLocation);
}

void AItem::SetLocation(const FVector& NewLocation) {
  CurrentLocation = NewLocation;
  SetActorLocation(CurrentLocation);
  DELTA_LOG("{}", DeltaFormat("CurrentLocation: {}", CurrentLocation.ToString()));
}

void AItem::SetRotation(const FRotator& NewRotation) {
  CurrentRotation = NewRotation;
  SetActorRotation(CurrentRotation);
  DELTA_LOG("{}", DeltaFormat("CurrentRotation: {}", CurrentRotation.ToString()));
}

void AItem::SetForwardDirection(const FVector& NewForwardDirection) {
  ForwardDirection = NewForwardDirection.GetSafeNormal();
  DELTA_LOG("{}", DeltaFormat("ForwardDirection: {}", ForwardDirection.ToString()));
}
