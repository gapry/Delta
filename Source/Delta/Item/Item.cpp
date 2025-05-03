// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "../Common/LogUtil.h"

AItem::AItem() {
  PrimaryActorTick.bCanEverTick = true;
}

void AItem::BeginPlay() {
  Super::BeginPlay();

  DELTA_LOG("{}", TCHAR_TO_UTF8(DELTA_FUNCSIG));
}

void AItem::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
}
