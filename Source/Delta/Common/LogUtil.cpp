// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "LogUtil.h"

#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"

DECLARE_LOG_CATEGORY_EXTERN(DeltaLog, Log, All);
DEFINE_LOG_CATEGORY(DeltaLog)

void LogUtil::PrintMessage(const int32    key,
                           const FString& Message,
                           const float    Duration,
                           const FColor   Color) {
  UE_LOG(DeltaLog, Warning, TEXT("%s"), *Message);

  if (GEngine) {
    GEngine->AddOnScreenDebugMessage(key, Duration, Color, Message);
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
