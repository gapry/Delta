// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "EchoCharacter.h"

AEchoCharacter::AEchoCharacter() {
  PrimaryActorTick.bCanEverTick = true;
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
