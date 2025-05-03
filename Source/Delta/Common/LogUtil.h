// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StringFormat.h"
#include "Logging/LogMacros.h"

#if defined(_MSC_VER)
#define DELTA_FUNCSIG TCHAR_TO_UTF8(TEXT(__FUNCSIG__))
#elif defined(__clang__) || defined(__GNUC__)
#define DELTA_FUNCSIG TCHAR_TO_UTF8(TEXT(__PRETTY_FUNCTION__))
#else
#define DELTA_FUNCSIG TCHAR_TO_UTF8(TEXT(__FUNCTION__))
#endif

class DELTA_API LogUtil {
public:
  static void PrintMessage(const int32    key,
                           const FString& Message,
                           const float    Duration = 5.f,
                           const FColor   Color    = FColor::Green);

  static void PrintMessage(const UObject* const Context,
                           const FString&       Message,
                           const FColor         Color    = FColor::Yellow,
                           const float          Duration = 5.f);

  template<class... ARGS>
  FORCEINLINE static void PrintMessage(const int32 key,
                                       const float Duration,
                                       const char* format_str,
                                       const ARGS&... args) {
    FString Message;
    fmt::format_to(DeltaFormatFStringBackInserter(Message),
                   fmt::runtime(format_str),
                   args...);
    PrintMessage(key, Message, Duration);
  }
};

#define DELTA_DEFAULT_LOG_DURATION 5.f

#define DELTA_LOG(...)                                \
  do {                                                \
    LogUtil::PrintMessage(INDEX_NONE,                 \
                          DELTA_DEFAULT_LOG_DURATION, \
                          __VA_ARGS__);               \
  } while (false)