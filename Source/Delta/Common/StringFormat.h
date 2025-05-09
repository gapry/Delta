// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include <UObject/ReflectedTypeAccessors.h>
#include <fmt/core.h>
#include <exception>

template<class T>
FORCEINLINE FName DeltaEnumToFName(const T& enumValue) {
  return StaticEnum<T>()->GetNameByValue(static_cast<int64>(enumValue));
}

class DELTA_API DeltaFormatFStringBackInserter {
  using This = DeltaFormatFStringBackInserter;

public:
  DeltaFormatFStringBackInserter() = default;

  explicit DeltaFormatFStringBackInserter(FString& s) noexcept
    : _s(&s) {
  }

  This& operator=(const char& _Val) {
    _s->AppendChar(_Val);
    return *this;
  }

  This& operator=(typename char&& _Val) {
    _s->AppendChar(std::move(_Val));
    return *this;
  }

  This& operator=(const wchar_t& _Val) {
    _s->AppendChar(_Val);
    return *this;
  }

  This& operator*() noexcept {
    return *this;
  }

  This& operator++() noexcept {
    return *this;
  }

  This operator++(int) noexcept {
    return *this;
  }

  FString* _s;
};

template<>
struct fmt::detail::is_output_iterator<DeltaFormatFStringBackInserter, char> : std::true_type {};

template<class... ARGS>
FORCEINLINE void
DeltaAppendFormat(FString& outStr, fmt::format_string<ARGS...> format_str, ARGS&&... args) {
  try {
    fmt::format_to(DeltaFormatFStringBackInserter(outStr), format_str, std::forward<ARGS>(args)...);
  } catch (const std::exception& e) {
    UE_LOG(LogTemp, Error, TEXT("Exception in DeltaAppendFormat: %s"), *FString(e.what()));
  }
}

template<class... ARGS>
FORCEINLINE FString DeltaFormat(fmt::format_string<ARGS...> format_str, ARGS&&... args) {
  FString tmp;
  DeltaAppendFormat(tmp, format_str, std::forward<ARGS>(args)...);
  return tmp;
}

template<class... ARGS>
FORCEINLINE FText DeltaFormatText(const char* format_str, const ARGS&... args) {
  FString tmp;
  DeltaAppendFormat(tmp, format_str, args...);
  return FText::FromString(tmp);
}

template<class... ARGS>
FORCEINLINE FName DeltaFormatName(const char* format_str, const ARGS&... args) {
  FString tmp;
  DeltaAppendFormat(tmp, format_str, args...);
  return FName(tmp);
}

struct DeltaFormatterBase {
  constexpr auto parse(fmt::format_parse_context& ctx) -> decltype(ctx.begin()) {
    return ctx.begin();
  }
};

template<>
struct fmt::formatter<wchar_t> : public DeltaFormatterBase {
  auto format(const wchar_t& v, fmt::format_context& ctx) {
    return ctx.out();
  }
};

template<>
struct fmt::formatter<FString> : public DeltaFormatterBase {
  auto format(const FString& v, fmt::format_context& ctx) const {
    auto it = *ctx.out();
    for (const auto& c : v.GetCharArray()) {
      it = static_cast<char>(c);
      it++;
    }
    return ctx.out();
  }
};

template<>
struct fmt::formatter<FName> : fmt::formatter<FString> {
  auto format(const FName& v, fmt::format_context& ctx) const {
    return fmt::formatter<FString>::format(v.ToString(), ctx);
  }
};

template<>
struct fmt::formatter<FSmartName> : fmt::formatter<FString> {
  auto format(const FSmartName& v, fmt::format_context& ctx) {
    return fmt::formatter<FString>::format(v.DisplayName.ToString(), ctx);
  }
};

template<>
struct fmt::formatter<FText> : fmt::formatter<FString> {
  auto format(const FText& v, fmt::format_context& ctx) {
    return fmt::formatter<FString>::format(v.ToString(), ctx);
  }
};

template<>
struct fmt::formatter<FVector2D> : public DeltaFormatterBase {
  auto format(const FVector2D& v, fmt::format_context& ctx) {
    return fmt::format_to(ctx.out(), "[{}, {}]", v.X, v.Y);
  }
};

template<>
struct fmt::formatter<FVector> : public DeltaFormatterBase {
  auto format(const FVector& v, fmt::format_context& ctx) {
    return fmt::format_to(ctx.out(), "[{}, {}, {}]", v.X, v.Y, v.Z);
  }
};

template<>
struct fmt::formatter<FVector_NetQuantize> : public fmt::formatter<FVector> {
  auto format(const FVector_NetQuantize& v, fmt::format_context& ctx) {
    return fmt::formatter<FVector>::format(v, ctx);
  }
};

template<>
struct fmt::formatter<FVector4> : public DeltaFormatterBase {
  auto format(const FVector4& v, fmt::format_context& ctx) {
    return fmt::format_to(ctx.out(), "[{}, {}, {}, {}]", v.X, v.Y, v.Z, v.W);
  }
};

template<>
struct fmt::formatter<FRotator> : public DeltaFormatterBase {
  auto format(const FRotator& v, fmt::format_context& ctx) {
    return fmt::format_to(ctx.out(), "[{}, {}, {}]", v.Pitch, v.Yaw, v.Roll);
  }
};

template<>
struct fmt::formatter<AActor> : public DeltaFormatterBase {
  auto format(const AActor& v, fmt::format_context& ctx) {
    return fmt::format_to(ctx.out(), "Actor:{}", &v == nullptr ? v.GetFName() : "");
  }
};