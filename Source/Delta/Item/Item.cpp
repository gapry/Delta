// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Item.h"
#include "Components/SphereComponent.h"
#include "../Common/LogUtil.h"

AItem::AItem() {
  {
    PrimaryActorTick.bCanEverTick = true;
  }

  {
    static const TCHAR* const ComponentName = TEXT("StaticMeshComponent");
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(ComponentName);
  }

  {
    if (StaticMeshComponent) {
      RootComponent = StaticMeshComponent;
    }
  }

  {
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
    SphereComponent->SetupAttachment(GetRootComponent());
  }
}

void AItem::OnConstruction(const FTransform& Transform) {
  Super::OnConstruction(Transform);
}

void AItem::BeginPlay() {
  Super::BeginPlay();

  SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereBeginOverlap);
  SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

void AItem::PostInitializeStaticMeshComponent() {
  if (!StaticMeshComponent) {
    DELTA_LOG("{}", DeltaFormat("StaticMeshComponent is null"));
    return;
  }
  StaticMeshComponent->SetMobility(EComponentMobility::Movable);
}

void AItem::PostInitializeRootComponent() {
  if (!RootComponent) {
    DELTA_LOG("{}", DeltaFormat("RootComponent is null"));
    return;
  }
  RootComponent->SetMobility(EComponentMobility::Movable);
}

void AItem::PostInitializeComponents() {
  Super::PostInitializeComponents();

  PostInitializeStaticMeshComponent();
  PostInitializeRootComponent();
}

void AItem::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
  RunningTime += DeltaTime;
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

float AItem::GetSineOscillationOffset() const {
  return FMath::Sin(2.f * PI * Frequency * RunningTime) * Amplitude;
}

float AItem::GetCosineOscillationOffset() const {
  return FMath::Cos(2.f * PI * Frequency * RunningTime) * Amplitude;
}

void AItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                 AActor*              OtherActor,
                                 UPrimitiveComponent* OtherComp,
                                 int32                OtherBodyIndex,
                                 bool                 bFromSweep,
                                 const FHitResult&    SweepResult) {
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
                               AActor*              OtherActor,
                               UPrimitiveComponent* OtherComp,
                               int32                OtherBodyIndex) {
}