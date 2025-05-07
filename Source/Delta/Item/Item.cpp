// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Item.h"
#include "../Common/LogUtil.h"
#include "../Common/DebugShape.h"

AItem::AItem() {
  PrimaryActorTick.bCanEverTick = true;

  InitializeStaticMeshComponent();
  InitializeCollision();
  InitializeRootComponent();
}

void AItem::InitializeStaticMeshComponent() {
  StaticMeshPath = TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_QuadPyramid'");

  static const TCHAR* const ComponentName = TEXT("StaticMeshComponent");
  StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(ComponentName);

  static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(StaticMeshPath);
  if (StaticMesh.Succeeded()) {
    StaticMeshComponent->SetStaticMesh(StaticMesh.Object);
  }
}

void AItem::BeginPlay() {
  Super::BeginPlay();

  SetLocation(FVector(0.f, 0.f, 50.f));
  SetRotation(FRotator(0.f, 45.f, 0.f));
  UpdateForwardDirection();
}

float AItem::GetSineOscillationOffset() const {
  return FMath::Sin(2.f * PI * Frequency * RunningTime) * Amplitude;
}

float AItem::GetCosineOscillationOffset() const {
  return FMath::Cos(2.f * PI * Frequency * RunningTime) * Amplitude;
}

void AItem::InitializeCollision() {
  StaticMeshComponent->SetCollisionProfileName(TEXT("Custom"));
  StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
  StaticMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
  StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,
                                                     ECollisionResponse::ECR_Ignore);
  StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,
                                                     ECollisionResponse::ECR_Overlap);
}

void AItem::InitializeRootComponent() {
  if (RootComponent) {
    RootComponent->SetMobility(EComponentMobility::Movable);
  }
  RootComponent = StaticMeshComponent;
}

void AItem::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
  RunningTime += DeltaTime;

  RenderDebugShapeOneFrame(DeltaTime);
}

void AItem::RenderDebugShape() const {
  const FVector CurrentLocation = GetActorLocation();

  DELTA_DEBUG_SPHERE_ONE_FRAME(CurrentLocation);
  DELTA_DEBUG_ARROW_ONE_FRAME(CurrentLocation,
                              CurrentLocation + GetActorForwardVector().GetSafeNormal() * 100.f);
}

void AItem::RenderDebugShapeOneFrame(const float DeltaTime) {
  // Frequency * RunningTime
  //      (Hz) * (s)         = (1 / s) * (s) = 1 = scalar = unitless
  const float DeltaZ = GetSineOscillationOffset();

  // MovementRate * DeltaTime
  //       (cm/s) * (s/frame) = (cm/frame)
  AddActorWorldOffset(FVector(MovementRate * DeltaTime, 0.f, DeltaZ));

  // RotationRate * DeltaTime
  //  (degrees/s) * (s/frame) = (degrees/frame)
  AddActorWorldRotation(FRotator(0.f, RotationRate * DeltaTime, 0.f));

  RenderDebugShape();
}

void AItem::SetLocation(const FVector& NewLocation) {
  SetActorLocation(NewLocation);
  DELTA_LOG("{}", DeltaFormat("CurrentLocation: {}", GetActorLocation().ToString()));
}

void AItem::SetRotation(const FRotator& NewRotation) {
  SetActorRotation(NewRotation);
  DELTA_LOG("{}", DeltaFormat("CurrentRotation: {}", GetActorRotation().ToString()));
}

void AItem::UpdateForwardDirection() {
  ForwardDirection = GetActorForwardVector().GetSafeNormal();
  DELTA_LOG("{}", DeltaFormat("ForwardDirection: {}", ForwardDirection.ToString()));
}
