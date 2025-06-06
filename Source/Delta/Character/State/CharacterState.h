// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8 {
  ECS_Unequipped              UMETA(DisplayName = "Unequipped"),
  ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
  ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon"),
};
