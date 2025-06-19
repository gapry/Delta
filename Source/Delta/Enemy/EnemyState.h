// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

UENUM(BlueprintType)
enum class EDeathPose : uint8 {
  EDP_Alive                UMETA(DisplayName = "Alive"),
  EDP_Death_FallingBack    UMETA(DisplayName = "DeathFallingBack"),
  EDP_Death_FallingForward UMETA(DisplayName = "DeathFallingForward"),
  EDP_Death_FlyingBack     UMETA(DisplayName = "DeathFlyingBack"),
  EDP_Death_ToRight        UMETA(DisplayName = "DeathToRight"),
  EDP_Death_ToFront        UMETA(DisplayName = "DeathToFront"),
  EDP_Death_ToBack         UMETA(DisplayName = "DeathToBack")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8 {
  EES_Dead       UMETA(DisplayName = "Dead"),
  EES_Patrolling UMETA(DisplayName = "Patrolling"),
  EES_Chasing    UMETA(DisplayName = "Chasing"),
  EES_Attacking  UMETA(DisplayName = "Attacking"),
  EES_Engaged    UMETA(DisplayName = "Engaged")
};
