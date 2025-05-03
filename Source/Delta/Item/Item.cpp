// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Item.h"
#include "../Common/LogUtil.h"

AItem::AItem() {
  PrimaryActorTick.bCanEverTick = true;
}

void AItem::BeginPlay() {
  Super::BeginPlay();

  DELTA_LOG("{}", DELTA_FUNCSIG);
}

void AItem::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  DELTA_LOG_ONCE("{}", DeltaFormat("DeltaTime = {}", DeltaTime));
}
