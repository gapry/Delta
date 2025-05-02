// Fill out your copyright notice in the Description page of Project Settings.

#include "LogUtil.h"
#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"

#include <fmt/core.h>

void LogUtil::PrintMessage(const FString& Message, const FColor Color, const float Duration) {
  UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);

  if (GEngine) {
    std::string FormattedMessage = fmt::format("Hello, {}! The answer is {}", "fmt", 2025);
    FString     TestFmtMessage(UTF8_TO_TCHAR(FormattedMessage.c_str()));

    GEngine->AddOnScreenDebugMessage(-1, Duration, Color, Message);
    GEngine->AddOnScreenDebugMessage(-1, Duration, Color, TestFmtMessage);
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
