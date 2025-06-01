// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "DrawDebugHelpers.h"

#define DELTA_INTERNAL_DEBUG_SPHERE(CurrentLocation, IsPersistentFlag)                                                   \
  do {                                                                                                                   \
    if (const UWorld* const CurrentWorld = GetWorld(); CurrentWorld != nullptr) {                                        \
      const float   SphereRadius   = 25.f;                                                                               \
      const int32   SphereSegments = 12;                                                                                 \
      const FColor& SphereColor    = FColor::Red;                                                                        \
      const bool    IsPersistent   = IsPersistentFlag;                                                                   \
      const float   LifeTime       = -1.f;                                                                               \
      DrawDebugSphere(CurrentWorld, CurrentLocation, SphereRadius, SphereSegments, SphereColor, IsPersistent, LifeTime); \
    }                                                                                                                    \
  } while (false)

#define DELTA_DEBUG_SPHERE(CurrentLocation)           DELTA_INTERNAL_DEBUG_SPHERE(CurrentLocation, true)

#define DELTA_DEBUG_SPHERE_COLOR(Location, Color)     DrawDebugSphere(GetWorld(), Location, 8.f, 12, Color, false, 5.f);

#define DELTA_DEBUG_SPHERE_ONE_FRAME(CurrentLocation) DELTA_INTERNAL_DEBUG_SPHERE(CurrentLocation, false)

#define DELTA_INTERNAL_DEBUG_LINE(StartLocation, EndLocation, IsPersistentFlag)                                             \
  do {                                                                                                                      \
    if (const UWorld* const CurrentWorld = GetWorld(); CurrentWorld != nullptr) {                                           \
      const FColor& LineColor     = FColor::Green;                                                                          \
      const bool    IsPersistent  = IsPersistentFlag;                                                                       \
      const float   LifeTime      = -1.f;                                                                                   \
      const uint8   DepthPriority = 0;                                                                                      \
      const float   Thickness     = 1.f;                                                                                    \
      DrawDebugLine(CurrentWorld, StartLocation, EndLocation, LineColor, IsPersistent, LifeTime, DepthPriority, Thickness); \
    }                                                                                                                       \
  } while (false)

#define DELTA_DEBUG_LINE(StartLocation, EndLocation)           DELTA_INTERNAL_DEBUG_LINE(StartLocation, EndLocation, true)

#define DELTA_DEBUG_LINE_ONE_FRAME(StartLocation, EndLocation) DELTA_INTERNAL_DEBUG_LINE(StartLocation, EndLocation, false)

#define DELTA_INTERNAL_DEBUG_POINT(Location, IsPersistentFlag)                                              \
  do {                                                                                                      \
    if (const UWorld* const CurrentWorld = GetWorld(); CurrentWorld != nullptr) {                           \
      const float   PointSize     = 15.f;                                                                   \
      const FColor& PointColor    = FColor::Blue;                                                           \
      const bool    IsPersistent  = IsPersistentFlag;                                                       \
      const float   LifeTime      = -1.f;                                                                   \
      const uint8   DepthPriority = 0;                                                                      \
      DrawDebugPoint(CurrentWorld, Location, PointSize, PointColor, IsPersistent, LifeTime, DepthPriority); \
    }                                                                                                       \
  } while (false)

#define DELTA_DEBUG_POINT(Location)           DELTA_INTERNAL_DEBUG_POINT(Location, true)

#define DELTA_DEBUG_POINT_ONE_FRAME(Location) DELTA_INTERNAL_DEBUG_POINT(Location, false)

#define DELTA_INTERNAL_DEBUG_ARROW(Start, End, IsPersistentFlag)                                                                    \
  do {                                                                                                                              \
    if (const UWorld* const CurrentWorld = GetWorld(); CurrentWorld != nullptr) {                                                   \
      const float   ArrowSize     = 30.f;                                                                                           \
      const FColor& ArrowColor    = FColor::Yellow;                                                                                 \
      const bool    IsPersistent  = IsPersistentFlag;                                                                               \
      const float   LifeTime      = -1.f;                                                                                           \
      const uint8   DepthPriority = 0;                                                                                              \
      const float   Thickness     = 2.f;                                                                                            \
      DrawDebugDirectionalArrow(CurrentWorld, Start, End, ArrowSize, ArrowColor, IsPersistent, LifeTime, DepthPriority, Thickness); \
    }                                                                                                                               \
  } while (false)

#define DELTA_DEBUG_ARROW(Start, End)           DELTA_INTERNAL_DEBUG_ARROW(Start, End, true)

#define DELTA_DEBUG_ARROW_ONE_FRAME(Start, End) DELTA_INTERNAL_DEBUG_ARROW(Start, End, false)