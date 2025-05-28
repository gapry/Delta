// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

UENUM(BlueprintType)
enum class EActionState : uint8 {
  EAS_Unoccupied      UMETA(DisplayName = "Unoccupied"),
  EAS_Attacking       UMETA(DisplayName = "Attacking"),
  EAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon"),
};