// Fill out your copyright notice in the Description page of Project Settings.

#include "LogUtil.h"
#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"

void LogUtil::PrintMessage(const FString& Message, const FColor Color, const float Duration) {
  UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);

  if (GEngine) {
    GEngine->AddOnScreenDebugMessage(-1, Duration, Color, Message);
  }
}

void LogUtil::PrintMessage(const UObject* const Context,
                           const FString&       Message,
                           const FColor         Color,
                           const float          Duration) {
  UKismetSystemLibrary::PrintString(Context,
                                    Message,
                                    true,
                                    true,
                                    Color.ReinterpretAsLinear(),
                                    Duration);
}
