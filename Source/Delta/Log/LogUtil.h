// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#if defined(_MSC_VER)
#define DELTA_FUNCSIG ANSI_TO_TCHAR(__FUNCSIG__)
#elif defined(__clang__) || defined(__GNUC__)
#define DELTA_FUNCSIG ANSI_TO_TCHAR(__PRETTY_FUNCTION__)
#else
#define DELTA_FUNCSIG ANSI_TO_TCHAR(__FUNCTION__)
#endif

class DELTA_API LogUtil {
public:
  static void PrintMessage(const UObject* const Context,
                           const FString&       Message,
                           const FColor         Color    = FColor::Green,
                           const float          Duration = 5.f);
};
