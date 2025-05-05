// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "DrawDebugHelpers.h"

#define DELTA_DEBUG_SPHERE(CurrentLocation)            \
  do {                                                 \
    if (const UWorld* const CurrentWorld = GetWorld(); \
        CurrentWorld != nullptr) {                     \
      const float   SphereRadius   = 25.f;             \
      const int32   SphereSegments = 12;               \
      const FColor& SphereColor    = FColor::Red;      \
      const bool    IsPersistent   = true;             \
      DrawDebugSphere(CurrentWorld,                    \
                      CurrentLocation,                 \
                      SphereRadius,                    \
                      SphereSegments,                  \
                      SphereColor,                     \
                      IsPersistent);                   \
    }                                                  \
  } while (false)

#define DELTA_DEBUG_LINE(StartLocation, EndLocation)   \
  do {                                                 \
    if (const UWorld* const CurrentWorld = GetWorld(); \
        CurrentWorld != nullptr) {                     \
      const FColor& LineColor         = FColor::Green; \
      const bool    IsPersistentLines = true;          \
      const float   LifeTime          = -1.f;          \
      const uint8   DepthPriority     = 0;             \
      const float   Thickness         = 1.f;           \
      DrawDebugLine(CurrentWorld,                      \
                    StartLocation,                     \
                    EndLocation,                       \
                    LineColor,                         \
                    IsPersistentLines,                 \
                    LifeTime,                          \
                    DepthPriority,                     \
                    Thickness);                        \
    }                                                  \
  } while (false)

#define DELTA_DEBUG_POINT(Location)                    \
  do {                                                 \
    if (const UWorld* const CurrentWorld = GetWorld(); \
        CurrentWorld != nullptr) {                     \
      const float   PointSize         = 15.f;          \
      const FColor& PointColor        = FColor::Blue;  \
      const bool    IsPersistentLines = true;          \
      const float   LifeTime          = -1.f;          \
      const uint8   DepthPriority     = 0;             \
      DrawDebugPoint(CurrentWorld,                     \
                     Location,                         \
                     PointSize,                        \
                     PointColor,                       \
                     IsPersistentLines,                \
                     LifeTime,                         \
                     DepthPriority);                   \
    }                                                  \
  } while (false)

#define DELTA_DEBUG_ARROW(Start, End)                   \
  do {                                                  \
    if (const UWorld* const CurrentWorld = GetWorld();  \
        CurrentWorld != nullptr) {                      \
      const float   ArrowSize         = 30.f;           \
      const FColor& ArrowColor        = FColor::Yellow; \
      const bool    IsPersistentLines = true;           \
      const float   LifeTime          = -1.f;           \
      const uint8   DepthPriority     = 0;              \
      const float   Thickness         = 2.f;            \
      DrawDebugDirectionalArrow(CurrentWorld,           \
                                Start,                  \
                                End,                    \
                                ArrowSize,              \
                                ArrowColor,             \
                                IsPersistentLines,      \
                                LifeTime,               \
                                DepthPriority,          \
                                Thickness);             \
    }                                                   \
  } while (false)