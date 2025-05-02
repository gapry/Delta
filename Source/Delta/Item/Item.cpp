// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "../Log/LogUtil.h"

AItem::AItem() {
  PrimaryActorTick.bCanEverTick = true;
}

void AItem::BeginPlay() {
  Super::BeginPlay();

  LogUtil::PrintMessage(this, DELTA_FUNCSIG);
}

void AItem::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
}
