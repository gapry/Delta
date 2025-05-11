// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "EchoCharacter.h"

#include "../Common/Finder.h"
#include "../Common/LogUtil.h"

#include "Components/SkeletalMeshComponent.h"

AEchoCharacter::AEchoCharacter() {
  PrimaryActorTick.bCanEverTick = true;

  {
    static constexpr const TCHAR* const SkeletalMeshPath{
      TEXT("/Script/Engine.SkeletalMesh'/Game/AncientContent/Characters/Echo/Meshes/Echo.Echo'")};

    Finder::SetSkeletalMesh(SkeletalMeshComponent.Get(), SkeletalMeshPath);
  }
}

void AEchoCharacter::PostInitializeComponents() {
  Super::PostInitializeComponents();

  PostInitializeSkeletalMeshComponent();
}

void AEchoCharacter::PostInitializeSkeletalMeshComponent() {
  if (!SkeletalMeshComponent.IsValid()) {
    DELTA_LOG("{}", DeltaFormat("[{}] {}", DELTA_FUNCSIG, "SkeletalMeshComponent is not valid"));
  }

  SkeletalMeshComponent->SetRelativeTransform(FTransform(FRotator(0.f, -90.f, 0.f), // Rotation
                                                         FVector(0.f, 0.f, -90.f),  // Translation
                                                         FVector(1.f, 1.f, 1.f)));  // Scale
}

void AEchoCharacter::BeginPlay() {
  Super::BeginPlay();
}

void AEchoCharacter::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
}

void AEchoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);
}
